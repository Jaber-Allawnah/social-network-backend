#include "./LikeRepository.hpp"
#include "../utils/DateTimeUtils.hpp"
#include "../utils/UserMapper.hpp"
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>

LikeRepository::LikeRepository(Database& database) : database_(database) {}

std::vector<User> LikeRepository::getByPostId(int postId) {
    try {
         mysqlx::Session& session = database_.getSession();
         mysqlx::SqlResult result = session.sql("SELECT u.id, u.username, u.email, u.password_hash, "
                                                "DATE_FORMAT(u.created_at, '%Y-%m-%d %H:%i:%s'), "
                                                "DATE_FORMAT(u.updated_at, '%Y-%m-%d %H:%i:%s') "
                                                "FROM likes l "
                                                "JOIN users u ON u.id = l.user_id "
                                                "WHERE l.post_id = ?").bind(postId).execute();
         auto rows = result.fetchAll();
         std::vector<User> users;
         for (const mysqlx::Row& row : rows) {
             users.push_back(mapRowToUser(row));
         }

         return users;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("Failed to retrieve users who liked the post by post id " + std::string(error.what()));
    }
}

void LikeRepository::like(int userId, int postId) {
    try {
         mysqlx::Session& session = database_.getSession();
         session.sql("INSERT INTO likes (user_id, post_id) "
                     "VALUES (?, ?)").bind(userId, postId).execute();
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("Failed to like the post " + std::string(error.what()));
    }
}

bool LikeRepository::unlike(int userId, int postId) {
    try {
         mysqlx::Session& session = database_.getSession();
         mysqlx::SqlResult result = session.sql("DELETE FROM likes "
                                                "WHERE post_id = ? AND user_id = ?").bind(postId, userId).execute();

         return result.getAffectedItemsCount() > 0;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("Failed to unlike the post " + std::string(error.what()));
    }
}