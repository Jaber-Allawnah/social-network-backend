#pragma once

#include "./Menu.hpp"
#include "../../services/LikeService.hpp"

class LikeMenu : public Menu {
private:
	LikeService& likeService_;

	void handleChoice(int choice, int currentUserId) override;
	void displayMenu() const override;

	void likePost(int currentUserId);
	void unlikePost(int currentUserId);

	void viewUsersWhoLikedPost(int currentUserId);

	void displayUser(const User& user) const;
public:
	LikeMenu(LikeService& likeSerive);
};