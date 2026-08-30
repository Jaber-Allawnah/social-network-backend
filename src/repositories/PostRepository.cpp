#include "./PostRepository.hpp"
#include "../utils/DateTimeUtils.hpp"
#include <mysqlx/xdevapi.h>
#include <stdexcept>
#include <spdlog/spdlog.h>

using namespace DateTimeUtils;

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
    spdlog::debug("Retrieving post {} from database", postId);

    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            SELECT
                id,
                content,
                user_id,
                DATE_FORMAT(created_at, '%Y-%m-%d %H:%i:%s'),
                DATE_FORMAT(updated_at, '%Y-%m-%d %H:%i:%s')
            FROM posts
            WHERE id = ?)";

        mysqlx::SqlResult result = session.sql(sql).bind(postId).execute();
        mysqlx::Row row = result.fetchOne();
        if (!row) {
            spdlog::debug("Post {} not found in database", postId);
            return std::nullopt;
        }

        Post post = mapRowToPost(row);

        spdlog::debug("Post {} retrieved from database", postId);

        return post;
    }
    catch (const mysqlx::Error& error) {
        spdlog::error("Failed to retrieve post {} from database: {}", postId, error.what());
        throw std::runtime_error("PostRepository: Failed to retrieve post by post id: " + std::string(error.what()));
    }
}

std::vector<Post> PostRepository::getByUserId(int userId) {
    spdlog::debug("Retrieving posts for user {} from database", userId);

    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            SELECT
                id,
                content,
                user_id,
                DATE_FORMAT(created_at, '%Y-%m-%d %H:%i:%s'),
                DATE_FORMAT(updated_at, '%Y-%m-%d %H:%i:%s')
            FROM posts
            WHERE user_id = ?)";

        mysqlx::SqlResult result = session.sql(sql).bind(userId).execute();
        auto rows = result.fetchAll();

        std::vector<Post> posts;
        for (const mysqlx::Row& row : rows) {
            posts.push_back(mapRowToPost(row));
        }

        spdlog::debug("Retrieved {} posts for user {} from database", posts.size(), userId);

        return posts;
    }
    catch (const mysqlx::Error& error) {
        spdlog::error("Failed to retrieve posts for user {} from database: {}", userId, error.what());
        throw std::runtime_error("PostRepository: Failed to retrieve posts by user id: " + std::string(error.what()));
    }
}

std::vector<Post> PostRepository::searchUserPosts(int userId, const std::string& keyword) {
    spdlog::debug("Searching posts for user {} in database", userId);

    try {
        mysqlx::Session& session = database_.getSession();
        std::string formattedKeyword = "%" + keyword + "%";
        const std::string sql = R"(
            SELECT
                id,
                content,
                user_id,
                DATE_FORMAT(created_at, '%Y-%m-%d %H:%i:%s'),
                DATE_FORMAT(updated_at, '%Y-%m-%d %H:%i:%s')
            FROM posts
            WHERE user_id = ?
               AND content LIKE ?)";

        mysqlx::SqlResult result = session.sql(sql).bind(userId, formattedKeyword).execute();
        auto rows = result.fetchAll();

        std::vector<Post> userPosts;
        for (const mysqlx::Row& row : rows) {
            userPosts.push_back(mapRowToPost(row));
        }

        spdlog::debug("Retrieved {} matching posts for user {} from database", userPosts.size(), userId);

        return userPosts;
    }
    catch (const mysqlx::Error& error) {
        spdlog::error("Failed to search posts for user {} in database: {}", userId, error.what());
        throw std::runtime_error("PostRepository: Failed to search user posts by keyword: " + std::string(error.what()));
    }
}

Post PostRepository::create(int userId, const std::string& content) {
    spdlog::debug("Creating post for user {} in database", userId);

    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            INSERT INTO posts (content, user_id)
            VALUES (?, ?))";

        mysqlx::SqlResult result = session.sql(sql).bind(content, userId).execute();

        int postId = static_cast<int>(result.getAutoIncrementValue());
        auto post = getById(postId);
        if (!post) {
            throw std::runtime_error("PostRepository: Created post could not be retrieved");
        }

        spdlog::debug("Post {} created for user {} in database", postId, userId);

        return post.value();
    }
    catch (const mysqlx::Error& error) {
        spdlog::error("Failed to create post for user {} in database: {}", userId, error.what());
        throw std::runtime_error("PostRepository: Failed to create post: " + std::string(error.what()));
    }
}

bool PostRepository::update(int postId, const std::string& content) {
    spdlog::debug("Updating post {} in database", postId);

    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            UPDATE posts
            SET content = ?
            WHERE id = ?)";

        mysqlx::SqlResult result = session.sql(sql).bind(content, postId).execute();

        bool updated = result.getAffectedItemsCount() > 0;
        spdlog::debug("Post {} update result: {}", postId, updated);

        return updated;
    }
    catch (const mysqlx::Error& error) {
        spdlog::error("Failed to update post {} in database: {}", postId, error.what());
        throw std::runtime_error("PostRepository: Failed to update post: " + std::string(error.what()));
    }
}

bool PostRepository::remove(int postId) {
    spdlog::debug("Removing post {} from database", postId);

    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            DELETE FROM posts
            WHERE id = ?)";

        mysqlx::SqlResult result = session.sql(sql).bind(postId).execute();

        bool removed = result.getAffectedItemsCount() > 0;
        spdlog::debug("Post {} removal result: {}", postId, removed);

        return removed;
    }
    catch (const mysqlx::Error& error) {
        spdlog::error("Failed to remove post {} from database: {}", postId, error.what());
        throw std::runtime_error("PostRepository: Failed to remove post: " + std::string(error.what()));
    }
}

std::vector<Post> PostRepository::getUserFeed(int userId) {
    spdlog::debug("Retrieving feed for user {} from database", userId);

    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            SELECT
                p.id,
                p.content,
                p.user_id,
                DATE_FORMAT(p.created_at, '%Y-%m-%d %H:%i:%s') AS created_at,
                DATE_FORMAT(p.updated_at, '%Y-%m-%d %H:%i:%s') AS updated_at
            FROM posts p
            WHERE p.user_id = ?
               OR p.user_id IN (
                   SELECT followee_id
                   FROM follows
                   WHERE follower_id = ?
               )
            ORDER BY p.created_at DESC)";

        mysqlx::SqlResult result = session.sql(sql).bind(userId, userId).execute();
        auto rows = result.fetchAll();

        std::vector<Post> posts;
        for (const mysqlx::Row& row : rows) {
            posts.push_back(mapRowToPost(row));
        }

        spdlog::debug("Retrieved {} feed posts for user {} from database", posts.size(), userId);

        return posts;
    }
    catch (const mysqlx::Error& error) {
        spdlog::error("Failed to retrieve feed for user {} from database: {}", userId, error.what());
        throw std::runtime_error("PostRepository: Failed to retrieve user feed: " + std::string(error.what()));
    }
}