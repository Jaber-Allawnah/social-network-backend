#pragma once

#include "IPostRepository.hpp"
#include "../database/Database.hpp"
#include <mysqlx/xdevapi.h>
#include <optional>
#include <vector>
#include <string>


class PostRepository : public IPostRepository{
private:
	Database& database_;

	Post mapRowToPost(const mysqlx::Row& row) const;

public:
	PostRepository(Database& database);

	std::optional<Post> getById(int postId) override;
	std::vector<Post> getByUserId(int userId) override;
	std::vector<Post> searchUserPosts(int userId, const std::string& content) override;
	std::vector<Post> getUserFeed(int userId) override;

	Post create(int userId, const std::string& content) override;
	bool update(int postId, const std::string& content) override ;
	bool remove(int postId) override;
};