#include "./CommentRepository.hpp"
#include "../utils/DateTimeUtils.hpp"
#include <stdexcept>
#include <spdlog/spdlog.h>

using namespace DateTimeUtils;

CommentRepository::CommentRepository(Database& database) : database_(database) {}

Comment CommentRepository::mapRowToComment(const mysqlx::Row& row) const {
    Comment comment;
    comment.id = static_cast<int>(row[0]);
    comment.content = static_cast<std::string>(row[1]);
    comment.userId = static_cast<int>(row[2]);
    comment.postId = static_cast<int>(row[3]);
    comment.createdAt = parseDateTime(static_cast<std::string>(row[4]));
    comment.updatedAt = parseDateTime(static_cast<std::string>(row[5]));

    return comment;
}

std::vector<Comment> CommentRepository::getByPostId(int postId) {
    spdlog::debug("Retrieving comments for post {} from database", postId);

    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            SELECT
                id,
                content,
                user_id,
                post_id,
                DATE_FORMAT(created_at, '%Y-%m-%d %H:%i:%s'),
                DATE_FORMAT(updated_at, '%Y-%m-%d %H:%i:%s')
            FROM comments
            WHERE post_id = ?)";

        mysqlx::SqlResult result = session.sql(sql).bind(postId).execute();
        auto rows = result.fetchAll();

        std::vector<Comment> postComments;
        for (const mysqlx::Row& row : rows) {
            postComments.push_back(mapRowToComment(row));
        }

        spdlog::debug("Retrieved {} comments for post {} from database", postComments.size(), postId);

        return postComments;
    }
    catch (const mysqlx::Error& error) {
        spdlog::error("Failed to retrieve comments for post {} from database: {}", postId, error.what());
        throw std::runtime_error("CommentRepository: Failed to retrieve comments by post id: " + std::string(error.what()));
    }
}

std::optional<Comment> CommentRepository::getById(int commentId) {
    spdlog::debug("Retrieving comment {} from database", commentId);

    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            SELECT
                id,
                content,
                user_id,
                post_id,
                DATE_FORMAT(created_at, '%Y-%m-%d %H:%i:%s'),
                DATE_FORMAT(updated_at, '%Y-%m-%d %H:%i:%s')
            FROM comments
            WHERE id = ?)";

        mysqlx::SqlResult result = session.sql(sql).bind(commentId).execute();
        mysqlx::Row row = result.fetchOne();
        if (!row) {
            spdlog::debug("Comment {} not found in database", commentId);
            return std::nullopt;
        }

        Comment comment = mapRowToComment(row);

        spdlog::debug("Comment {} retrieved from database", commentId);

        return comment;
    }
    catch (const mysqlx::Error& error) {
        spdlog::error("Failed to retrieve comment {} from database: {}", commentId, error.what());
        throw std::runtime_error("CommentRepository: Failed to retrieve comment by comment id: " + std::string(error.what()));
    }
}

Comment CommentRepository::create(int userId, int postId, const std::string& content) {
    spdlog::debug("Creating comment for user {} on post {} in database", userId, postId);

    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            INSERT INTO comments (content, user_id, post_id)
            VALUES(?, ?, ?))";

        mysqlx::SqlResult result = session.sql(sql).bind(content, userId, postId).execute();

        int commentId = static_cast<int>(result.getAutoIncrementValue());
        auto comment = getById(commentId);
        if (!comment) {
            throw std::runtime_error("CommentRepository: Created comment could not be retrieved");
        }

        spdlog::debug("Comment {} created in database for user {} on post {}", commentId, userId, postId);

        return comment.value();
    }
    catch (const mysqlx::Error& error) {
        spdlog::error("Failed to create comment for user {} on post {} in database: {}", userId, postId, error.what());
        throw std::runtime_error("CommentRepository: Failed to create comment: " + std::string(error.what()));
    }
}

bool CommentRepository::update(int commentId, const std::string& content) {
    spdlog::debug("Updating comment {} in database", commentId);

    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            UPDATE comments
            SET content = ?
            WHERE id = ?)";

        mysqlx::SqlResult result = session.sql(sql).bind(content, commentId).execute();

        bool updated = result.getAffectedItemsCount() > 0;
        if (updated) {
            spdlog::debug("Comment {} update result in database: {}", commentId, updated);
        }

        return updated;
    }
    catch (const mysqlx::Error& error) {
        spdlog::error("Failed to update comment {} in database: {}", commentId, error.what());
        throw std::runtime_error("CommentRepository: Failed to update comment: " + std::string(error.what()));
    }
}

bool CommentRepository::remove(int commentId) {
    spdlog::debug("Removing comment {} from database", commentId);

    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            DELETE FROM comments
            WHERE id = ?)";

        mysqlx::SqlResult result = session.sql(sql).bind(commentId).execute();

        bool removed = result.getAffectedItemsCount() > 0;
        if (removed) {
            spdlog::debug("Comment {} removal result in database: {}", commentId, removed);
        }

        return removed;
    }
    catch (const mysqlx::Error& error) {
        spdlog::error("Failed to remove comment {} from database: {}", commentId, error.what());
        throw std::runtime_error("CommentRepository: Failed to remove comment: " + std::string(error.what()));
    }
}