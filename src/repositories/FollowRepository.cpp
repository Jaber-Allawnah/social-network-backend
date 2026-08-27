#include "./FollowRepository.hpp"
#include "../utils/DateTimeUtils.hpp"
#include "../utils/UserMapper.hpp"
#include <stdexcept>

using namespace UserMapper;
FollowRepository::FollowRepository(Database& database) : database_(database) {}

bool FollowRepository::follow(int followerId, int followeeId) {
    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            INSERT INTO follows (follower_id, followee_id)
            VALUES (?, ?))";

        mysqlx::SqlResult result = session.sql(sql).bind(followerId, followeeId).execute();

        return result.getAffectedItemsCount() > 0;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("FollowRepository: Failed to follow user: " + std::string(error.what()));
    }
}

bool FollowRepository::unfollow(int followerId, int followeeId) {
    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            DELETE FROM follows
            WHERE follower_id = ?
               AND followee_id = ?)";

        mysqlx::SqlResult result = session.sql(sql).bind(followerId, followeeId).execute();

        return result.getAffectedItemsCount() > 0;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("FollowRepository: Failed to unfollow user: " + std::string(error.what()));
    }
}

std::vector<User> FollowRepository::getFollowers(int userId) {
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
            FROM follows f
            JOIN users u ON u.id = f.follower_id
            WHERE f.followee_id = ?)";

        mysqlx::SqlResult result = session.sql(sql).bind(userId).execute();
        auto rows = result.fetchAll();

        std::vector<User> followers;
        for (const mysqlx::Row& row : rows) {
            followers.push_back(mapRowToUser(row));
        }

        return followers;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("FollowRepository: Failed to retrieve followers: " + std::string(error.what()));
    }
}

std::vector<User> FollowRepository::getFollowing(int userId) {
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
            FROM follows f
            JOIN users u ON u.id = f.followee_id
            WHERE f.follower_id = ?)";

        mysqlx::SqlResult result = session.sql(sql).bind(userId).execute();
        auto rows = result.fetchAll();

        std::vector<User> following;
        for (const mysqlx::Row& row : rows) {
            following.push_back(mapRowToUser(row));
        }

        return following;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("FollowRepository: Failed to retrieve following: " + std::string(error.what()));
    }
}

bool FollowRepository::isFollowing(int followerId, int followeeId) {
    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            SELECT 1
            FROM follows
            WHERE follower_id = ?
               AND followee_id = ?)";

        mysqlx::SqlResult result = session.sql(sql).bind(followerId, followeeId).execute();
        mysqlx::Row row = result.fetchOne();
        if (!row)
            return false;

        return true;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("FollowRepository: Failed to retrieve following: " + std::string(error.what()));
    }
}