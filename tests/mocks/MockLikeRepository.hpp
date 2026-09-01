#pragma once

#include <gmock/gmock.h>
#include "../../src/repositories/ILikeRepository.hpp"

class MockLikeRepository : public ILikeRepository {
public:
    MOCK_METHOD(std::vector<User>, getByPostId, (int postId), (override));
    MOCK_METHOD(bool, hasLiked, (int userId, int postId), (override));
    MOCK_METHOD(bool, like, (int userId, int postId), (override));
    MOCK_METHOD(bool, unlike, (int userId, int postId), (override));
};