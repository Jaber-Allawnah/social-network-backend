#pragma once

#include "../database/Database.hpp"
#include "../models/Like.hpp"
#include <string>
#include <mysqlx/xdevapi.h>
#include <vector>
#include <chrono>

class LikeRepository {
private:
	Database& database_;

	Like mapRowToLike(const mysqlx::Row& row) const;

public:
	LikeRepository(Database& database);
	
	// Get Operation
	std::vector<Like> getByPostId(int postId);

	// Add Operation
	void addLike(int userId, int postId);

	// Delete Operation
	void deleteLike(int userId, int postId);
};