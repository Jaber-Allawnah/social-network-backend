#include "./CommentRepository.hpp"
#include "../utils/DateTimeUtils.hpp"
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>

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
    try {
        mysqlx::Session& session = database_.getSession();
        mysqlx::SqlResult result = session.sql("SELECT id, content, user_id, post_id, "
                                               "DATE_FORMAT(created_at, '%Y-%m-%d %H:%i:%s'), "
                                               "DATE_FORMAT(updated_at, '%Y-%m-%d %H:%i:%s') "
                                               "FROM comments "
                                               "WHERE post_id = ?").bind(postId).execute();
        auto rows = result.fetchAll();
        std::vector<Comment> postComments;
        for (const mysqlx::Row& row : rows) {
            postComments.push_back(mapRowToComment(row));
        }

        return postComments;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("Failed to retrieve comment by post id" + std::string(error.what()));
    }
}

std::optional<Comment> CommentRepository::getById(int commentId) {
    try {
        mysqlx::Session& session = database_.getSession();
        mysqlx::SqlResult result = session.sql("SELECT id, content, user_id, post_id, "
                                               "DATE_FORMAT(created_at, '%Y-%m-%d %H:%i:%s'), "
                                               "DATE_FORMAT(updated_at, '%Y-%m-%d %H:%i:%s') "
                                               "FROM comments "
                                               "WHERE id = ?").bind(commentId).execute();
        mysqlx::Row row= result.fetchOne();
        if (!row)
            return std::nullopt;
        Comment comment = mapRowToComment(row);

        return comment;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("Failed to retrieve comment by comment id" + std::string(error.what()));
    }
}

void CommentRepository::create(const std::string& content, int userId, int postId) {
    try {
        mysqlx::Session& session = database_.getSession();
        session.sql("INSERT INTO comments (content, user_id, post_id) "
                    "VALUES(?, ?, ?)").bind(content, userId, postId).execute();
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("Failed to add comment" + std::string(error.what()));
    }
}

bool CommentRepository::update(int commentId, const std::string& content) {
    try {
        mysqlx::Session& session = database_.getSession();
        mysqlx::SqlResult result = session.sql("UPDATE comments "
                                               "SET content = ? "
                                               "WHERE id = ?").bind(content, commentId).execute();

        return result.getAffectedItemsCount() > 0;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("Failed to update comment" + std::string(error.what()));
    }
}

bool CommentRepository::remove(int commentId) {
    try {
        mysqlx::Session& session = database_.getSession();
        mysqlx::SqlResult result = session.sql("DELETE FROM comments "
                                               "WHERE id = ?").bind(commentId).execute();

        return result.getAffectedItemsCount() > 0;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("Failed to update comment" + std::string(error.what()));
    }
}