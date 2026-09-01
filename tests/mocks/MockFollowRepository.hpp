#pragma once

#include <gmock/gmock.h>
#include "../../src/repositories/IFollowRepository.hpp"

class MockFollowRepository : public IFollowRepository {
public:
    MOCK_METHOD(bool, follow, (int followerId, int followeeId), (override));
    MOCK_METHOD(bool, unfollow, (int followerId, int followeeId), (override));
    MOCK_METHOD(bool, isFollowing, (int followerId, int followeeId), (override));

    MOCK_METHOD(std::vector<User>, getFollowers, (int userId), (override));
    MOCK_METHOD(std::vector<User>, getFollowing, (int userId), (override));
};