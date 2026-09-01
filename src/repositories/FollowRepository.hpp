#pragma once

#include "../database/Database.hpp"
#include "./IFollowRepository.hpp"
#include "../models/Follow.hpp"
#include "../models/User.hpp"
#include <mysqlx/xdevapi.h>
#include <vector>

class FollowRepository : public IFollowRepository {
private:
	Database& database_;

public:
	FollowRepository(Database& database);

	bool follow(int followerId, int followeeId) override;
	bool unfollow(int followerId, int followeeId) override;
	bool isFollowing(int followerId, int followeeId) override;

	std::vector<User> getFollowers(int userId) override;
	std::vector<User> getFollowing(int userId) override;
};
