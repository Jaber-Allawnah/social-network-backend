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
	ConnectionMenu& connectionMenu_;

	int readChoice() const;
	void displayMenu() const;
	LoggedOutResult handleChoice(int choice);

	std::optional<User> registerUser();
	std::optional<User> login();

	void displayUser(const User& user) const;

public:
	LoggedOutMenu(UserService& userService, 
				  ConnectionMenu& connectionMenu);

	LoggedOutResult run();
};