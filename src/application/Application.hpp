#pragma once

#include "./LoggedOutmenu.hpp"
#include "./LoggedInMenu.hpp"
#include <optional>

class Application {
private:
	LoggedOutMenu& loggedOutMenu_;
	LoggedInMenu& loggedInMenu_;
	std::optional<User> currentUser_ = std::nullopt;
	bool isRunning_= true;

public:
	Application(LoggedOutMenu& loggedOutMenu, LoggedInMenu& loggedInMenu);

	void run();
};