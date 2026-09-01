#pragma once

#include "../models/Like.hpp"
#include "../models/User.hpp"
#include <vector>

class ILikeRepository {
public:
    virtual ~ILikeRepository() = default;

	virtual std::vector<User> getByPostId(int postId) = 0;

	virtual bool hasLiked(int userId, int postId) = 0;

	virtual bool like(int userId, int postId) = 0;
	virtual bool unlike(int userId, int postId) = 0;
};