#include "Application.hpp"
#include <iostream>

Application::Application(LoggedOutMenu& loggedOutMenu, 
						 LoggedInMenu& loggedInMenu)
                      :	 loggedOutMenu_(loggedOutMenu), 
						 loggedInMenu_(loggedInMenu){}

void Application::run() {
	while (isRunning_) {
		try {
			 if (!currentUser_) {
				 LoggedOutResult loggedOutResult = loggedOutMenu_.run();
				 currentUser_ = loggedOutResult.user;
				 isRunning_ = !loggedOutResult.shouldExit;
			 }
			 else {
				 LoggedInResult loggedInResult = loggedInMenu_.run(currentUser_->id);
				 if (loggedInResult.shouldLogout)
					 currentUser_ = std::nullopt;
				 isRunning_ = !loggedInResult.shouldExit;
			 }
		}
		catch (const std::exception& error) {
			std::cout << "Error: " << error.what() << "\n";
		}
	}
}