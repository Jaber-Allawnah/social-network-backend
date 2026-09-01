#pragma once

#include "../database/Database.hpp"
#include "../models/Comment.hpp"
#include "./ICommentRepository.hpp"
#include <vector>
#include <optional>
#include <mysqlx/xdevapi.h>

class CommentRepository : public ICommentRepository {
private:
	Database& database_;

	Comment mapRowToComment(const mysqlx::Row& row) const;

public:
	CommentRepository(Database& database);

	std::vector<Comment> getByPostId(int postId) override;
	std::optional<Comment> getById(int commentId) override;

    Comment create(int userId, int postId, const std::string& content) override;
	bool update(int commentId, const std::string& content) override;
	bool remove(int commentId) override;
};