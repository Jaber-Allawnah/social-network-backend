#include "ConnectionMenu.hpp"
#include <iostream>

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
              << "3. Back\n"
              << "Enter your choice: ";
}

void ConnectionMenu::viewFollowers(int currentUserId) {
    std::vector<User> followers = followService_.getFollowers(currentUserId);

    if (followers.empty()) {
        std::cout << "You have no followers.\n";
        return;
    }

    for (const User& user : followers) {
        displayUser(user);
    }
}

void ConnectionMenu::viewFollowing(int currentUserId) {
    std::vector<User> following = followService_.getFollowing(currentUserId);

    if (following.empty()) {
        std::cout << "You are not following anyone.\n";
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
        break;
    default:
        std::cout << "Please enter a valid option.\n";
        break;
    }
}