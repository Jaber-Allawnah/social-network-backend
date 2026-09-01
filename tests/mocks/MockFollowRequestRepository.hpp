#pragma once

#include <gmock/gmock.h>
#include "../../src/repositories/IFollowRequestRepository.hpp"

class MockFollowRequestRepository : public IFollowRequestRepository {
public:
    MOCK_METHOD(bool, create, (int requesterId, int receiverId), (override));

    MOCK_METHOD(bool, update, (int followRequestId, FollowRequestStatus status), (override));

    MOCK_METHOD(std::vector<FollowRequest>, getByRequesterId, (int requesterId), (override));
    MOCK_METHOD(std::vector<FollowRequest>, getByReceiverId, (int receiverId), (override));
    MOCK_METHOD(std::vector<FollowRequest>, getUserPendingRequests, (int userId), (override));
    MOCK_METHOD(std::optional<FollowRequest>, getById, (int followRequestId), (override));
    MOCK_METHOD(std::optional<FollowRequest>, getByUsers, (int requesterId, int receiverId),(override));
};