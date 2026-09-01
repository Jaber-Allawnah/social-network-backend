#pragma once

#include <gmock/gmock.h>
#include "../../src/repositories/ICommentRepository.hpp"

class MockCommentRepository : public ICommentRepository {
public:
    MOCK_METHOD(std::vector<Comment>, getByPostId, (int postId), (override));
    MOCK_METHOD(std::optional<Comment>, getById, (int commentId), (override));
    MOCK_METHOD(Comment, create, (int userId, int postId, const std::string& content), (override));
    MOCK_METHOD(bool, update, (int commentId, const std::string& content), (override));
    MOCK_METHOD(bool, remove, (int commentId), (override));
};