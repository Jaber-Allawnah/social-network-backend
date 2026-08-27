#pragma once

#include "Menu.hpp"
#include "../../services/FollowService.hpp"

class ConnectionMenu : public Menu {
private:
    FollowService& followService_;

    void displayMenu() const override;
    void handleChoice(int choice, int currentUserId) override;

    void displayUser(const User& user) const;

public:
    ConnectionMenu(FollowService& followService);

    void viewFollowers(int userId);
    void viewFollowing(int userId);
};