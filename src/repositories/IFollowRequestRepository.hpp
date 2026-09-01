#pragma once

#include "../models/FollowRequest.hpp"
#include <optional>
#include <vector>

class IFollowRequestRepository {
public:
    virtual ~IFollowRequestRepository() = default;

    virtual bool create(int requesterId, int receiverId) = 0;
    virtual bool update(int followRequestId, FollowRequestStatus status) = 0;

    virtual std::vector<FollowRequest> getByRequesterId(int requesterId) = 0;
    virtual std::vector<FollowRequest> getByReceiverId(int receiverId) = 0;
    virtual std::vector<FollowRequest> getUserPendingRequests(int userId) = 0;
    virtual std::optional<FollowRequest> getById(int followRequestId) = 0;
    virtual std::optional<FollowRequest> getByUsers(int requesterId, int receiverId) = 0;
};