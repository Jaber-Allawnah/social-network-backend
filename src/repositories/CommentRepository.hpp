#pragma once

#include "../database/Database.hpp"
#include "../models/Comment.hpp"
#include <vector>
#include <optional>
#include <mysqlx/xdevapi.h>

class CommentRepository {
private:
	Database& database_;
	Comment mapRowToComment(const mysqlx::Row& row) const;

public:
	CommentRepository(Database& database);

	std::vector<Comment> getByPostId(int postId);
	std::optional<Comment> getById(int commentId);

    void create(const std::string& content, int userId, int postId);
	bool update(int commentId, const std::string& content);
	bool remove(int commentId);
};