#pragma once

#include <vector>
#include "../models/User.hpp"

class IFollowRepository {
public:
    virtual ~IFollowRepository() = default;

	virtual bool follow(int followerId, int followeeId) = 0;
	virtual bool unfollow(int followerId, int followeeId) = 0;
	virtual bool isFollowing(int followerId, int followeeId) = 0;

	virtual std::vector<User> getFollowers(int userId) = 0;
	virtual std::vector<User> getFollowing(int userId) = 0;
};