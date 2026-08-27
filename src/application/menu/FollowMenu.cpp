#include "FollowMenu.hpp"
#include "../../utils/MenuInputUtils.hpp"
#include "../../utils/FollowRequestStatusUtils.hpp"
#include "../../utils/DateTimeUtils.hpp"
#include <iostream>

using namespace MenuInputUtils;
using namespace DateTimeUtils;
using namespace FollowRequestStatusUtils;

FollowMenu::FollowMenu(FollowService& followService) : followService_(followService) {}

void FollowMenu::displayMenu() const {
    std::cout << "\nFollow Menu\n"
              << "1. Send Follow Request\n"
              << "2. Accept Follow Request\n"
              << "3. Reject Follow Request\n"
              << "4. Unfollow User\n"
              << "5. View Pending Incoming Requests\n"
              << "6. Back\n"
              << "Enter your choice: ";
}

void FollowMenu::sendFollowRequest(int currentUserId) {
    int receiverId = readId("Enter the ID of the user you want to follow: ");

    if (!followService_.sendFollowRequest(currentUserId, receiverId)) {
        std::cout << "Follow request failed.\n";
        return;
    }
    std::cout << "Follow request sent successfully.\n";
}

void FollowMenu::unfollowUser(int currentUserId) {
    int followeeId = readId("Enter the ID of the user you want to unfollow: ");

    if (!followService_.unfollow(currentUserId, followeeId)) {
        std::cout << "Unfollow failed.\n";
        return;
    }
    std::cout << "User unfollowed successfully.\n";
}

void FollowMenu::acceptFollowRequest(int currentUserId) {
    int requestId = readId("Enter the ID of the follow request you want to accept: ");

    if (!followService_.acceptFollowRequest(requestId, currentUserId)) {
        std::cout << "Failed to accept follow request.\n";
        return;
    }
    std::cout << "Follow request accepted successfully.\n";
}

void FollowMenu::rejectFollowRequest(int currentUserId) {
    int requestId = readId("Enter the ID of the follow request you want to reject: ");

    if (!followService_.rejectFollowRequest(requestId, currentUserId)) {
        std::cout << "Failed to reject follow request.\n";
        return;
    }
    std::cout << "Follow request rejected successfully.\n";
}

void FollowMenu::displayFollowRequest(const FollowRequest& request) const {
    std::cout << "Request ID: " << request.id
              << " | Requester ID: " << request.requesterId
              << " | Receiver ID: " << request.receiverId
              << " | Status: " << FollowRequestStatusUtils::followRequestStatusToString(request.status)
              << " | Created At: " << DateTimeUtils::formatDateTime(request.createdAt)
              << " | Updated At: " << DateTimeUtils::formatDateTime(request.updatedAt)
              << '\n';

}
void FollowMenu::viewPendingIncomingRequests(int currentUserId) {
    std::vector<FollowRequest> pendingFollowRequests = followService_.getPendingIncomingRequests(currentUserId);
    if (pendingFollowRequests.empty()) {
        std::cout << "You have no pending follow requests \n";
        return;
    }
    
    for (const FollowRequest& followRequest : pendingFollowRequests) {
        displayFollowRequest(followRequest);
    }
}

void FollowMenu::handleChoice(int choice, int currentUserId) {
    switch (choice) {
    case 1:
        sendFollowRequest(currentUserId);
        break;
    case 2:
        acceptFollowRequest(currentUserId);
        break;
    case 3:
        rejectFollowRequest(currentUserId);
        break;
    case 4:
        unfollowUser(currentUserId);
        break;
    case 5:
        viewPendingIncomingRequests(currentUserId);
        break;
    case 6:
        break;
    default:
        std::cout << "Please enter a valid option.\n";
        break;
    }
}