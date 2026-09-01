#pragma once

#include <gmock/gmock.h>
#include "../../src/repositories/IPostRepository.hpp"

class MockPostRepository : public IPostRepository {
public:
	MOCK_METHOD(std::optional<Post>, getById, (int postId), (override));
	MOCK_METHOD(std::vector<Post>, getByUserId, (int userId), (override));
    MOCK_METHOD(std::vector<Post>, searchUserPosts, (int userId, const std::string& content), (override));
    MOCK_METHOD(std::vector<Post>, getUserFeed, (int userId), (override));

    MOCK_METHOD(Post, create, (int userId, const std::string& content), (override));
    MOCK_METHOD(bool, update, (int postId, const std::string& content), (override));
    MOCK_METHOD(bool, remove, (int postId), (override));
};