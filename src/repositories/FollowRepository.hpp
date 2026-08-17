#pragma once

#include "../database/Database.hpp"
#include "../models/Follow.hpp"
#include <mysqlx/xdevapi.h>
#include <vector>
#include <chrono>

class FollowRepository {
private:
	Database& database_;

public:
	FollowRepository(Database& datebase);
	// Add Operation
	void addFollow(int followerId, int followeeId);

	// Delete Operation
	void deleteFollow(int followerId, int followeeId);

	// Get Operation
	std::vector<int> getFollowerIds(int userId);
	std::vector<int> getFollowingIds(int userId);
};
