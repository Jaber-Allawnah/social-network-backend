#include "./PostRepository.hpp"
#include "../utils/DateTimeUtils.hpp"
#include <mysqlx/xdevapi.h>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>

PostRepository::PostRepository(Database& database) : database_(database) {}

Post PostRepository::mapRowToPost(const mysqlx::Row& row) const {
    Post post;
    post.id = static_cast<int>(row[0]);
    post.content = static_cast<std::string>(row[1]);
    post.userId = static_cast<int>(row[2]);
    post.createdAt = parseDateTime(static_cast<std::string>(row[3]));
    post.updatedAt = parseDateTime(static_cast<std::string>(row[4]));

    return post;
}

std::optional<Post> PostRepository::getById(int postId) {
    try {
         mysqlx::Session& session = database_.getSession();
         mysqlx::SqlResult result = session.sql("SELECT id, content, user_id, "
                                                "DATE_FORMAT(created_at, '%Y-%m-%d %H:%i:%s'), "
                                                "DATE_FORMAT(updated_at, '%Y-%m-%d %H:%i:%s') "
                                                "FROM posts "
                                                "WHERE id = ?").bind(postId).execute();
         mysqlx::Row row = result.fetchOne();
         if (!row) {
             return std::nullopt;
         }
         Post post = mapRowToPost(row);

         return post;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("Failed to retrieve post by post id " + std::string(error.what()));
    }
}

std::vector<Post> PostRepository::getByUserId(int userId) {
    try {
         mysqlx::Session& session = database_.getSession();
         mysqlx::SqlResult result = session.sql("SELECT id, content, user_id, "
                                                "DATE_FORMAT(created_at, '%Y-%m-%d %H:%i:%s'), "
                                                "DATE_FORMAT(updated_at, '%Y-%m-%d %H:%i:%s') "
                                                "FROM posts "
                                                "WHERE user_id = ?").bind(userId).execute();
         auto rows = result.fetchAll();
         std::vector<Post> posts;
         for (const mysqlx::Row& row : rows) {
             posts.push_back(mapRowToPost(row));
         }

         return posts;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("Failed to retrieve post by user id " + std::string(error.what()));
    }
}

std::vector<Post> PostRepository::searchUserPosts(int userId, const std::string& keyword) {
    try {
         mysqlx::Session& session = database_.getSession();
         std::string formattedKeyword = "%" + keyword + "%";
         mysqlx::SqlResult result = session.sql("SELECT id, content, user_id, "
                                                "DATE_FORMAT(created_at, '%Y-%m-%d %H:%i:%s'), "
                                                "DATE_FORMAT(updated_at, '%Y-%m-%d %H:%i:%s') "
                                                "FROM posts "
                                                "WHERE user_id = ? AND content LIKE ?").bind(userId, formattedKeyword).execute();
         auto rows = result.fetchAll();
         std::vector<Post> userPosts;
         for (const mysqlx::Row& row : rows) {
             userPosts.push_back(mapRowToPost(row));
         }

        return userPosts;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("Failed to get post by user id " + std::string(error.what()));
    }
}

void PostRepository::create(int userId, const std::string& content) {
    try {
         mysqlx::Session& session = database_.getSession();
         session.sql("INSERT INTO posts (content, user_id) "
                     "VALUES (?, ?)").bind(content, userId).execute();
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("Failed create post " + std::string(error.what()));
    }
}

bool PostRepository::update(int postId, const std::string& content) {
    try {
         mysqlx::Session& session = database_.getSession();
         mysqlx::SqlResult result = session.sql("UPDATE posts "
                                                "SET content = ? "
                                                "WHERE id = ?").bind(content, postId).execute();
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("Failed updated post " + std::string(error.what()));
    }
}

bool PostRepository::remove(int postId) {
    try {
         mysqlx::Session& session = database_.getSession();
         mysqlx::SqlResult result = session.sql("DELETE FROM posts "
                                                "WHERE id = ?").bind(postId).execute();

        return result.getAffectedItemsCount() > 0;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("Failed delete post " + std::string(error.what()));
    }
}


