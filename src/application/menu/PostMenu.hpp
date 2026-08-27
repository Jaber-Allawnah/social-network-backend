#pragma once

#include "Menu.hpp"
#include "../../services/PostService.hpp"

class PostMenu : public Menu {
private:
	PostService& postService_;

	void handleChoice(int choice, int currentUserId) override;
	void displayMenu() const override;

	void createPost(int currentUserId);
	void updatePost(int currentUserId);
	void deletePost(int currentUserId);

	void viewPost(int currentUserId);
	void viewMyPosts(int currentUserId);
	void viewUserPosts(int currentUserId);
	void searchUserPosts(int currentUserId);

	void displayPost(const Post& post) const;
public:
	PostMenu(PostService& postService);
};