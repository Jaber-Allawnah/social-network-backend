#include "./FollowRequestStatusUtils.hpp"
#include <stdexcept>
#include <spdlog/spdlog.h>

namespace FollowRequestStatusUtils {
    FollowRequestStatus parseFollowRequestStatus(const std::string& status) {
        spdlog::debug("Parsing follow request status");

        if (status == "pending") {
            spdlog::debug("Follow request status parsed successfully as pending");
            return FollowRequestStatus::Pending;
        }

        if (status == "accepted") {
            spdlog::debug("Follow request status parsed successfully as accepted");
            return FollowRequestStatus::Accepted;
        }

        if (status == "rejected") {
            spdlog::debug("Follow request status parsed successfully as rejected");
            return FollowRequestStatus::Rejected;
        }

        spdlog::warn("Invalid follow request status value: {}", status);
        throw std::runtime_error("Invalid follow request status: " + status);
    }

    std::string followRequestStatusToString(FollowRequestStatus status) {
        spdlog::debug("Converting follow request status to string");

        switch (status) {
        case FollowRequestStatus::Pending:
            spdlog::debug("Follow request status converted successfully to pending");
            return "pending";
        case FollowRequestStatus::Accepted:
            spdlog::debug("Follow request status converted successfully to accepted");
            return "accepted";
        case FollowRequestStatus::Rejected:
            spdlog::debug("Follow request status converted successfully to rejected");
            return "rejected";
        default:
            spdlog::warn("Invalid follow request status enum value");
            throw std::runtime_error("Invalid follow request status");
        }
    }
}