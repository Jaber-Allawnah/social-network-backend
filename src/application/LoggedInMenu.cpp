#include "LoggedInMenu.hpp"
#include ".././utils/DateTimeUtils.hpp"
#include <iostream>

using namespace DateTimeUtils;

LoggedInMenu::LoggedInMenu(Menu& postMenu,
						   Menu& commentMenu,
						   Menu& likeMenu,
						   Menu& followMenu,
						   Menu& connectionMenu, 
                           PostService& postService)
						:  postMenu_(postMenu),
						   commentMenu_(commentMenu),
						   likeMenu_(likeMenu),
						   followMenu_(followMenu),
						   connectionMenu_(connectionMenu), 
                           postService_(postService) {}

void LoggedInMenu::displayMenu() const {
	std::cout << "\nLogged In Menu\n"
  	          << "1. Posts\n"
		      << "2. Comments\n"
		      << "3. Likes\n"
		      << "4. Follow Actions\n"
	          << "5. Feed\n"
		      << "6. Followers / Following\n"
		      << "7. Logout\n"
	          << "8. Exit\n"
	          << "Enter your choice: ";
}

int LoggedInMenu::readChoice() const {
	int choice;
	while (!(std::cin >> choice)) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		std::cout << "Invalid input. Please enter a number: ";
	}

	return choice;
}

void LoggedInMenu::viewFeed(int currentUserId) {
    std::vector<Post> userFeed = postService_.getFeed(currentUserId);
    if (userFeed.empty()) {
        std::cout << "Your feed is empty \n";
    }

    for (const Post& post : userFeed) {
        displayPost(post);
    }
}

void LoggedInMenu::displayPost(const Post& post) const {
    std::cout << "Post ID: " << post.id
              << " | User ID: " << post.userId
              << " | Content: " << post.content
              << " | Created At: " << formatDateTime(post.createdAt)
              << " | Updated At: " << formatDateTime(post.updatedAt)
              << '\n';
}

LoggedInResult LoggedInMenu::handleChoice(int choice, int currentUserId) {
    LoggedInResult result;

    switch (choice) {
    case 1:
        postMenu_.run(currentUserId);
        break;
    case 2:
        commentMenu_.run(currentUserId);
        break;
    case 3:
        likeMenu_.run(currentUserId);
        break;
    case 4:
        followMenu_.run(currentUserId);
        break;
    case 5:
        viewFeed(currentUserId);
        break;
    case 6:
        connectionMenu_.run(currentUserId);
        break;
    case 7:
        result.shouldLogout = true;
        break;
    case 8:
        result.shouldExit = true;
        break;
    default:
        std::cout << "Please enter a valid option.\n";
        break;
    }

    return result;
}

LoggedInResult LoggedInMenu::run(int currentUserId) {
	displayMenu();
	int choice = readChoice();
	return handleChoice(choice, currentUserId);
}