#pragma once

#include "./menu/Menu.hpp"
#include "../services/PostService.hpp"

struct LoggedInResult {
	bool shouldLogout = false;
	bool shouldExit = false;
};

class LoggedInMenu {
private:
	Menu& postMenu_;
	Menu& commentMenu_;
	Menu& likeMenu_;
	Menu& followMenu_;
	Menu& connectionMenu_;

	PostService& postService_;

	int readChoice() const;
	void displayMenu() const;
	LoggedInResult handleChoice(int choice, int currentUserId);

	void viewFeed(int currentUserId);
	void displayPost(const Post& post) const;

public:
	LoggedInMenu(Menu& postMenu, 
			 	 Menu& commentMenu, 
				 Menu& likeMenu, 
				 Menu& followMenu, 
				 Menu& connectionMenu, 
				 PostService& postService_);

	LoggedInResult run(int currentUserId);
};