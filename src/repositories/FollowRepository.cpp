#include "./FollowRepository.hpp"
#include "../utils/DateTimeUtils.hpp"
#include "../utils/UserMapper.hpp"
#include <stdexcept>
#include <spdlog/spdlog.h>

using namespace UserMapper;
FollowRepository::FollowRepository(Database& database) : database_(database) {}

bool FollowRepository::follow(int followerId, int followeeId) {
    spdlog::debug("Creating follow relationship from user {} to user {} in database", followerId, followeeId);

    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            INSERT INTO follows (follower_id, followee_id)
            VALUES (?, ?))";

        mysqlx::SqlResult result = session.sql(sql).bind(followerId, followeeId).execute();

        bool followed = result.getAffectedItemsCount() > 0;
        spdlog::debug("Follow relationship creation result for user {} to user {}: {}", followerId, followeeId, followed);

        return followed;
    }
    catch (const mysqlx::Error& error) {
        spdlog::error("Failed to create follow relationship from user {} to user {} in database: {}", 
                      followerId, 
                      followeeId, 
                      error.what());
        throw std::runtime_error("FollowRepository: Failed to follow user: " + std::string(error.what()));
    }
}

bool FollowRepository::unfollow(int followerId, int followeeId) {
    spdlog::debug("Removing follow relationship from user {} to user {} in database", followerId, followeeId);

    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            DELETE FROM follows
            WHERE follower_id = ?
               AND followee_id = ?)";

        mysqlx::SqlResult result = session.sql(sql).bind(followerId, followeeId).execute();

        bool unfollowed = result.getAffectedItemsCount() > 0;
        spdlog::debug("Follow relationship removal result for user {} to user {}: {}", followerId, followeeId, unfollowed);

        return unfollowed;
    }
    catch (const mysqlx::Error& error) {
        spdlog::error("Failed to remove follow relationship from user {} to user {} in database: {}", 
                      followerId, 
                      followeeId, 
                      error.what());
        throw std::runtime_error("FollowRepository: Failed to unfollow user: " + std::string(error.what()));
    }
}

std::vector<User> FollowRepository::getFollowers(int userId) {
    spdlog::debug("Retrieving followers for user {} from database", userId);

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

        spdlog::debug("Retrieved {} followers for user {} from database", followers.size(), userId);

        return followers;
    }
    catch (const mysqlx::Error& error) {
        spdlog::error("Failed to retrieve followers for user {} from database: {}", userId, error.what());
        throw std::runtime_error("FollowRepository: Failed to retrieve followers: " + std::string(error.what()));
    }
}

std::vector<User> FollowRepository::getFollowing(int userId) {
    spdlog::debug("Retrieving following list for user {} from database", userId);

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

        spdlog::debug("Retrieved {} followed users for user {} from database", following.size(), userId);

        return following;
    }
    catch (const mysqlx::Error& error) {
        spdlog::error("Failed to retrieve following list for user {} from database: {}", userId, error.what());
        throw std::runtime_error("FollowRepository: Failed to retrieve following: " + std::string(error.what()));
    }
}

bool FollowRepository::isFollowing(int followerId, int followeeId) {
    spdlog::debug("Checking follow relationship from user {} to user {} in database", followerId, followeeId);

    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            SELECT 1
            FROM follows
            WHERE follower_id = ?
               AND followee_id = ?)";

        mysqlx::SqlResult result = session.sql(sql).bind(followerId, followeeId).execute();
        mysqlx::Row row = result.fetchOne();
        if (!row) {
            spdlog::debug("User {} is not following user {}", followerId, followeeId);
            return false;
        }

        spdlog::debug("User {} is following user {}", followerId, followeeId);

        return true;
    }
    catch (const mysqlx::Error& error) {
        spdlog::error("Failed to check follow relationship from user {} to user {} in database: {}", 
                      followerId, 
                      followeeId, 
                      error.what());
        throw std::runtime_error("FollowRepository: Failed to retrieve following: " + std::string(error.what()));
    }
}