#include "./FollowRequestStatusUtils.hpp"
#include <stdexcept>

FollowRequestStatus parseFollowRequestStatus(const std::string& status) {
    if (status == "pending")
        return FollowRequestStatus::Pending;

    if (status == "accepted")
        return FollowRequestStatus::Accepted;

    if (status == "rejected")
        return FollowRequestStatus::Rejected;

    throw std::runtime_error("Invalid follow request status: " + status);
}

std::string followRequestStatusToString(FollowRequestStatus status) {
    switch (status) {
    case FollowRequestStatus::Pending:
        return "pending";
    case FollowRequestStatus::Accepted:
        return "accepted";
    case FollowRequestStatus::Rejected:
        return "rejected";
    default:
        throw std::runtime_error("Invalid follow request status");
    }
}
