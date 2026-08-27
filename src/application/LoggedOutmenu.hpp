#pragma once

#include "../services/UserService.hpp"
#include "../services/FollowService.hpp"
#include "./menu/ConnectionMenu.hpp"

struct LoggedOutResult {
	std::optional<User> user;
	bool shouldExit = false;
};

class LoggedOutMenu {
private:
	UserService& userService_;
	FollowService& followService_;
	ConnectionMenu& connectionMenu_;

	int readChoice() const;
	void displayMenu() const;
	LoggedOutResult handleChoice(int choice);

	std::optional<User> registerUser();
	std::optional<User> login();

	void viewFollowers(int currentUserId);
	void viewFollowing(int currentUserId);
	void displayUser(const User& user) const;

public:
	LoggedOutMenu(UserService& userService, 
			      FollowService& followService,
				  ConnectionMenu& connectionMenu);

	LoggedOutResult run();
};