#include "./LikeRepository.hpp"
#include "../utils/DateTimeUtils.hpp"
#include "../utils/UserMapper.hpp"
#include <stdexcept>

LikeRepository::LikeRepository(Database& database) : database_(database) {}

std::vector<User> LikeRepository::getByPostId(int postId) {
    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            SELECT
                u.id,
                u.username,
                u.email,
                u.password_hash,
                DATE_FORMAT(u.created_at, '%Y-%m-%d %H:%i:%s'),
                DATE_FORMAT(u.updated_at, '%Y-%m-%d %H:%i:%s')
            FROM likes l
            JOIN users u ON u.id = l.user_id
            WHERE l.post_id = ?)";

        mysqlx::SqlResult result = session.sql(sql).bind(postId).execute();
        auto rows = result.fetchAll();

        std::vector<User> users;
        for (const mysqlx::Row& row : rows) {
            users.push_back(mapRowToUser(row));
        }

        return users;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("LikeRepository: Failed to retrieve users who liked the post by post id: " + std::string(error.what()));
    }
}

bool LikeRepository::like(int userId, int postId) {
    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            INSERT INTO likes (user_id, post_id)
            VALUES (?, ?))";

        mysqlx::SqlResult result = session.sql(sql).bind(userId, postId).execute();

        return result.getAffectedItemsCount() > 0;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("LikeRepository: Failed to like post: " + std::string(error.what()));
    }
}

bool LikeRepository::unlike(int userId, int postId) {
    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            DELETE FROM likes
            WHERE post_id = ?
               AND user_id = ?)";

        mysqlx::SqlResult result = session.sql(sql).bind(postId, userId).execute();

        return result.getAffectedItemsCount() > 0;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("LikeRepository: Failed to unlike post: " + std::string(error.what()));
    }
}

bool LikeRepository::hasLiked(int userId, int postId) {
    try {
        mysqlx::Session& session = database_.getSession();

        const std::string sql = R"(
            SELECT 1
            FROM likes
            WHERE user_id = ?
               AND post_id = ?
            LIMIT 1)";

        mysqlx::SqlResult result = session.sql(sql).bind(userId, postId).execute();

        return result.fetchOne();
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("LikeRepository: Failed to check if user liked post: " +
            std::string(error.what()));
    }
}