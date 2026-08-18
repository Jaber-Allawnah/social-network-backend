#pragma once

#include "../models/Post.hpp"
#include "../database/Database.hpp"
#include <mysqlx/xdevapi.h>
#include <optional>
#include <vector>
#include <string>


class PostRepository {
private:
	Database& database_;
	Post mapRowToPost(const mysqlx::Row& row) const;

public:
	PostRepository(Database& database);

	std::optional<Post> getById(int postId);
	std::vector<Post> getByUserId(int userId);
	std::vector<Post> searchUserPosts(int userId, const std::string& content);

	void create(int userId, const std::string& content);
	bool update(int postId, const std::string& content);
	bool remove(int postId);
};