#pragma once

#include "../models/Post.hpp"
#include "../database/Database.hpp"
#include <mysqlx/xdevapi.h>
#include <optional>
#include <vector>
#include <string>
#include <chrono>

class PostRepository {
private:
	Database& database_;

	Post mapRowToPost(const mysqlx::Row& row) const;

public:
	PostRepository(Database& database);

	// Get Operations
	std::optional<Post> getById(int id);
	std::vector<Post> getByUserId(int userId);
	std::vector<Post> searchUserPosts(int id, const std::string& content);

	// Add Operation
	void addPost(const std::string& content, int userId);

	// Update Operation
	void updatePost(int id, const std::string& content);

	// Delete Operation
	void deletePost(int id);
};