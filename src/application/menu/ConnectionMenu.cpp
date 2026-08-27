#include "ConnectionMenu.hpp"
#include "../../utils/MenuInputUtils.hpp"
#include <iostream>

using namespace MenuInputUtils;

ConnectionMenu::ConnectionMenu(FollowService& followService) : followService_(followService) {}

void ConnectionMenu::displayUser(const User& user) const {
    std::cout << "User ID: " << user.id
              << " | Username: " << user.username
              << " | Email: " << user.email
              << '\n';
}

void ConnectionMenu::displayMenu() const {
    std::cout << "\nConnection Menu\n"
              << "1. View Followers\n"
              << "2. View Following\n"
              << "3. View another user's followers\n"
              << "4. View another user's following\n"
              << "5. Back\n"
              << "Enter your choice: ";
}

void ConnectionMenu::viewFollowers(int userId) {
    std::vector<User> followers = followService_.getFollowers(userId);
    if (followers.empty()) {
        std::cout << "No followers.\n";
        return;
    }

    for (const User& user : followers) {
        displayUser(user);
    }
}

void ConnectionMenu::viewFollowing(int userId) {
    std::vector<User> following = followService_.getFollowing(userId);
    if (following.empty()) {
        std::cout << "Not following anyone.\n";
        return;
    }

    for (const User& user : following) {
        displayUser(user);
    }
}

void ConnectionMenu::handleChoice(int choice, int currentUserId) {
    switch (choice) {
    case 1:
        viewFollowers(currentUserId);
        break;
    case 2:
        viewFollowing(currentUserId);
        break;
    case 3:
        viewFollowers(readId("Please enter the user ID: "));
        break;
    case 4:
        viewFollowing(readId("Please enter the user ID: "));
        break;
    case 5:
        break;
    default:
        std::cout << "Please enter a valid option.\n";
        break;
    }
}