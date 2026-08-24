#include "./PostRepository.hpp"
#include "../utils/DateTimeUtils.hpp"
#include <mysqlx/xdevapi.h>
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
            return std::nullopt;
        }

        Post post = mapRowToPost(row);

        return post;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("PostRepository: Failed to retrieve post by post id: " + std::string(error.what()));
    }
}

std::vector<Post> PostRepository::getByUserId(int userId) {
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

        return posts;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("PostRepository: Failed to retrieve posts by user id: " + std::string(error.what()));
    }
}

std::vector<Post> PostRepository::searchUserPosts(int userId, const std::string& keyword) {
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

        return userPosts;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("PostRepository: Failed to search user posts by keyword: " + std::string(error.what()));
    }
}

Post PostRepository::create(int userId, const std::string& content) {
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

        return post.value();
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("PostRepository: Failed to create post: " + std::string(error.what()));
    }
}

bool PostRepository::update(int postId, const std::string& content) {
    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            UPDATE posts
            SET content = ?
            WHERE id = ?)";

        mysqlx::SqlResult result = session.sql(sql).bind(content, postId).execute();
        return result.getAffectedItemsCount() > 0;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("PostRepository: Failed to update post: " + std::string(error.what()));
    }
}

bool PostRepository::remove(int postId) {
    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            DELETE FROM posts
            WHERE id = ?)";

        mysqlx::SqlResult result = session.sql(sql).bind(postId).execute();

        return result.getAffectedItemsCount() > 0;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("PostRepository: Failed to remove post: " + std::string(error.what()));
    }
}

std::vector<Post> PostRepository::getUserFeed(int userId) {
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

        return posts;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("PostRepository: Failed to retrieve user feed: " + std::string(error.what()));
    }
}