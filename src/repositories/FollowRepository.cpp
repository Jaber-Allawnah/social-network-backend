#include "./FollowRepository.hpp"
#include "../utils/DateTimeUtils.hpp"
#include "../utils/UserMapper.hpp"
#include <stdexcept>

FollowRepository::FollowRepository(Database& database) : database_(database) {}

void FollowRepository::follow(int followerId, int followeeId) {
    try {
         mysqlx::Session& session = database_.getSession();
         session.sql("INSERT INTO follows (follower_id, followee_id) "
                     "VALUES (?, ?)").bind(followerId, followeeId).execute();
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("Failed follow user" + std::string(error.what()));
    }
}

bool FollowRepository::unfollow(int followerId, int followeeId) {
    try {
         mysqlx::Session& session = database_.getSession();
         mysqlx::SqlResult result = session.sql("DELETE FROM follows "
                                                "WHERE follower_id = ? AND followee_id = ?").bind(followerId, followeeId).execute();

        return result.getAffectedItemsCount() > 0;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("Failed follow user" + std::string(error.what()));
    }
}

std::vector<User> FollowRepository::getFollowers(int userId) {
    try {
         mysqlx::Session& session = database_.getSession();
         mysqlx::SqlResult result = session.sql("SELECT u.id, u.username, u.email, u.password_hash, "
                                                "DATE_FORMAT(u.created_at, '%Y-%m-%d %H:%i:%s'), "
                                                "DATE_FORMAT(u.updated_at, '%Y-%m-%d %H:%i:%s') "
                                                "FROM follows f "
                                                "JOIN users u ON u.id = f.follower_id "
                                                "WHERE f.followee_id = ?").bind(userId).execute();
         auto rows = result.fetchAll();
         std::vector<User> followers;
         for (const mysqlx::Row& row : rows) {
             followers.push_back(mapRowToUser(row));
         }

         return followers;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("Failed to retrieve followers " + std::string(error.what()));
    }
}

std::vector<User> FollowRepository::getFollowing(int userId) {
    try {
         mysqlx::Session& session = database_.getSession();
         mysqlx::SqlResult result = session.sql("SELECT u.id, u.username, u.email, u.password_hash, "
                                                "DATE_FORMAT(u.created_at, '%Y-%m-%d %H:%i:%s'), "
                                                "DATE_FORMAT(u.updated_at, '%Y-%m-%d %H:%i:%s') "
                                                "FROM follows f "
                                                "JOIN users u ON u.id = f.followee_id "
                                                "WHERE f.follower_id = ?").bind(userId).execute();
         auto rows = result.fetchAll();
         std::vector<User> following;
         for (const mysqlx::Row& row : rows) {
             following.push_back(mapRowToUser(row));
         }

        return following;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("Failed retrieve following " + std::string(error.what()));
    }
}

bool FollowRepository::isFollowing(int followerId, int followeeId) {
    try {
         mysqlx::Session& session = database_.getSession();
         mysqlx::SqlResult result = session.sql("SELECT 1 "
                                                "FROM follows "
                                                "WHERE follower_id = ? AND followee_id = ?").bind(followerId, followeeId).execute();
         mysqlx::Row row = result.fetchOne();
         if (!row)
             return false;
         return true;
     }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("Failed to check follow relationship " + std::string(error.what()));
    }
}