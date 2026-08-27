#include "LikeMenu.hpp"
#include "../../utils/MenuInputUtils.hpp"
#include <iostream>

using namespace MenuInputUtils;

LikeMenu::LikeMenu(LikeService& likeService) : likeService_(likeService) {}

void LikeMenu::displayMenu() const {
    std::cout << "\nLike Menu\n"
              << "1. Like Post\n"
              << "2. Unlike Post\n"
              << "3. View Users Who Liked a Post\n"
              << "4. Back\n"
              << "Enter your choice: ";
}

void LikeMenu::displayUser(const User& user) const {
    std::cout << "User ID: " << user.id
              << " | Username: " << user.username
              << " | Email: " << user.email
              << '\n';
}


void LikeMenu::likePost(int currentUserId) {
    int postId = readId("Enter the ID of the post you want to like: ");

    if (!likeService_.like(currentUserId, postId)) {
        std::cout << "Failed to like post.\n";
        return;
    }
    std::cout << "Post liked successfully.\n";
}

void LikeMenu::unlikePost(int currentUserId) {
    int postId = readId("Enter the ID of the post you want to unlike: ");

    if (!likeService_.unlike(currentUserId, postId)) {
        std::cout << "Failed to unlike post.\n";
        return;
    }
    std::cout << "Post unliked successfully.\n";
}

void LikeMenu::viewUsersWhoLikedPost(int currentUserId) {
    int postId = readId("Enter the ID of the post: ");

    std::vector<User> users = likeService_.getUsersWhoLikedPost(currentUserId, postId);
    if (users.empty()) {
        std::cout << "This post has no likes.\n";
        return;
    }

    for (const User& user : users) {
        displayUser(user);
    }
}


void LikeMenu::handleChoice(int choice, int currentUserId) {
    switch (choice) {
    case 1:
        likePost(currentUserId);
        break;
    case 2:
        unlikePost(currentUserId);
        break;
    case 3:
        viewUsersWhoLikedPost(currentUserId);
        break;
    case 4:
        break;
    default:
        std::cout << "Please enter a valid option.\n";
        break;
    }
}