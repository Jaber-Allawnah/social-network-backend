#pragma once

#include "Menu.hpp"
#include "../../services/FollowService.hpp"

class ConnectionMenu : public Menu {
private:
    FollowService& followService_;

    void displayMenu() const override;
    void handleChoice(int choice, int currentUserId) override;

    void viewFollowers(int currentUserId);
    void viewFollowing(int currentUserId);
    void displayUser(const User& user) const;

public:
    ConnectionMenu(FollowService& followService);
};