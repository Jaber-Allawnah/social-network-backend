#pragma once

#include "../models/Post.hpp"
#include <optional>
#include <vector>
#include <string>

class IPostRepository {
public:
	virtual ~IPostRepository() = default;

    virtual std::optional<Post> getById(int postId) = 0;
    virtual std::vector<Post> getByUserId(int userId) = 0;
    virtual std::vector<Post> searchUserPosts(int userId,
                                              const std::string& content) = 0;
    virtual std::vector<Post> getUserFeed(int userId) = 0;

    virtual Post create(int userId,
                        const std::string& content) = 0;
    virtual bool update(int postId,
                        const std::string& content) = 0;
    virtual bool remove(int postId) = 0;
};