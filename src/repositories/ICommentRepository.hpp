#pragma once

#include "../models/Comment.hpp"
#include <optional>
#include <string>
#include <vector>

class ICommentRepository {
public:
    virtual ~ICommentRepository() = default;

    virtual std::vector<Comment> getByPostId(int postId) = 0;
    virtual std::optional<Comment> getById(int commentId) = 0;

    virtual Comment create(int userId, int postId, const std::string& content) = 0;
    virtual bool update(int commentId, const std::string& content) = 0;
    virtual bool remove(int commentId) = 0;
};