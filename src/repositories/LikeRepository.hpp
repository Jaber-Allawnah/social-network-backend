#pragma once

#include "../database/Database.hpp"
#include "../models/Like.hpp"
#include "../models/User.hpp"
#include "./ILikeRepository.hpp"
#include <mysqlx/xdevapi.h>
#include <vector>

class LikeRepository : public ILikeRepository {
private:
	Database& database_;

public:
	LikeRepository(Database& database);
	
	std::vector<User> getByPostId(int postId) override;

	bool hasLiked(int userId, int postId) override;

	bool like(int userId, int postId) override;
	bool unlike(int userId, int postId) override;
};