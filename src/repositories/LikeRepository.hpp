#pragma once

#include "../database/Database.hpp"
#include "../models/Like.hpp"
#include "../models/User.hpp"
#include <mysqlx/xdevapi.h>
#include <vector>

class LikeRepository {
private:
	Database& database_;

public:
	LikeRepository(Database& database);
	
	std::vector<User> getByPostId(int postId);

	bool hasLiked(int userId, int postId);

	bool like(int userId, int postId);
	bool unlike(int userId, int postId);
};