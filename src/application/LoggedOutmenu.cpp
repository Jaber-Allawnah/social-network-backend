#include "LoggedOutMenu.hpp"
#include "../utils/MenuInputUtils.hpp"

#include <iostream>

using namespace MenuInputUtils;

LoggedOutMenu::LoggedOutMenu(UserService& userService, 
                             FollowService& followService,
                             ConnectionMenu& connectionMenu)
                           : userService_(userService),
                             followService_(followService),
                             connectionMenu_(connectionMenu){}

void LoggedOutMenu::displayMenu() const {
    std::cout << "\nLogged Out Menu\n"
              << "1. Register\n"
              << "2. Login\n"
              << "3. See user's followers\n"
              << "4. See user's following\n"
              << "5. Exit\n"
              << "Enter your choice: ";
}

int LoggedOutMenu::readChoice() const {
    int choice;

    while (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Invalid input. Please enter a number: ";
    }

    return choice;
}

LoggedOutResult LoggedOutMenu::handleChoice(int choice) {
    LoggedOutResult logoutResult;

    switch (choice) {
    case 1:
        logoutResult.user = registerUser();
        break;
    case 2:
        logoutResult.user = login();
        break;
    case 3:
        connectionMenu_.viewFollowers(readId("Please enter the user ID: "));
        break;
    case 4:
        connectionMenu_.viewFollowing(readId("Please enter the user ID: "));
        break;
    case 5:
        logoutResult.shouldExit = true;
        break;
    default:
        std::cout << "Please enter a valid option.\n";
        break;
    }

    return logoutResult;
}

LoggedOutResult LoggedOutMenu::run() {
    displayMenu();
    int choice = readChoice();
    return handleChoice(choice);
}

std::optional<User> LoggedOutMenu::registerUser() {
    std::string username;
    std::string email;
    std::string password;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter username: ";
    std::getline(std::cin, username);
    std::cout << "Enter email: ";
    std::getline(std::cin, email);
    std::cout << "Enter password: ";
    std::getline(std::cin, password);

    if (username.empty() || email.empty() || password.empty()) {
        std::cout << "Username, email, and password cannot be empty.\n";
        return std::nullopt;
    }

    return userService_.registerUser(username, email, password);
}

std::optional<User> LoggedOutMenu::login() {
    std::string email;
    std::string password;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Enter email: ";
    std::getline(std::cin, email);
    std::cout << "Enter password: ";
    std::getline(std::cin, password);

    if (email.empty() || password.empty()) {
        std::cout << "Email and password cannot be empty.\n";
        return std::nullopt;
    }

    return userService_.login(email, password);
}

void LoggedOutMenu::viewFollowers(int userId) {
    std::vector<User> followers = followService_.getFollowers(userId);

    if (followers.empty()) {
        std::cout << "This user does not have any followers.\n";
        return;
    }

    for (const User& user : followers) {
        displayUser(user);
    }
}

void LoggedOutMenu::viewFollowing(int userId) {
    std::vector<User> following = followService_.getFollowing(userId);

    if (following.empty()) {
        std::cout << "This user does not follow anyone\n";
        return;
    }

    for (const User& user : following) {
        displayUser(user);
    }
}

void LoggedOutMenu::displayUser(const User& user) const {
    std::cout << "User ID: " << user.id
              << " | Username: " << user.username
              << " | Email: " << user.email
              << '\n';
}