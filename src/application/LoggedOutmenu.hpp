#pragma once

#include "../services/UserService.hpp"

struct LoggedOutResult {
	std::optional<User> user;
	bool shouldExit = false;
};

class LoggedOutMenu {
private:
	UserService& userService_;

	int readChoice() const;
	void displayMenu() const;
	LoggedOutResult handleChoice(int choice);

	std::optional<User> registerUser();
	std::optional<User> login();

public:
	LoggedOutMenu(UserService& userService);

	LoggedOutResult run();
};