#pragma once

#include "../database/Database.hpp"
#include "../models/Follow.hpp"
#include "../models/User.hpp"
#include <mysqlx/xdevapi.h>
#include <vector>

class FollowRepository {
private:
	Database& database_;

public:
	FollowRepository(Database& database);

	void follow(int followerId, int followeeId);
	bool unfollow(int followerId, int followeeId);
	bool isFollowing(int followerId, int followeeId);

	std::vector<User> getFollowers(int userId);
	std::vector<User> getFollowing(int userId);
};
