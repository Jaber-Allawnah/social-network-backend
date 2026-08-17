#pragma once

#include "../database/Database.hpp"
#include "../models/Comment.hpp"
#include <vector>
#include <mysqlx/xdevapi.h>

class CommentRepository {
private:
	Database& database_;

	Comment mapRowToComment(const mysqlx::Row& row) const;

public:
	CommentRepository(Database& database);

	// Get Operation
	std::vector<Comment> getByPostId(int postId);

	// Add Operation
    void addComment(const std::string& content, int userId, int postId);

	// Update Operation
	void updateComment(int id, const std::string& content);

	// Delete Operation
	void deleteComment(int id);
};