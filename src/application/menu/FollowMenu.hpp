#pragma once

#include "./Menu.hpp"
#include "../../services/FollowService.hpp"
class FollowMenu : public Menu {
private:
	FollowService& followService_;

	void handleChoice(int choice, int currentUserId) override;
	void displayMenu() const override;

	void sendFollowRequest(int currentUserId);
	void unfollowUser(int currentUserId);

	void acceptFollowRequest(int currentUserId);
	void rejectFollowRequest(int currentUserId);

	void viewPendingIncomingRequests(int currentUserId);
	void displayFollowRequest(const FollowRequest& request) const;
public:
	FollowMenu(FollowService& followService);
};