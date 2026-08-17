#include "./CommentRepository.hpp"
#include "../utils/DateTimeUtils.hpp"
#include <ctime>
#include <iomanip>
#include <sstream>

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

void CommentRepository::addComment(const std::string& content, int userId, int postId) {
    mysqlx::Session& session = database_.getSession();

    session.sql("INSERT INTO comments (content, user_id, post_id) "
                "VALUES(?, ?, ?)").bind(content, userId, postId).execute();
}

void CommentRepository::updateComment(int id, const std::string& content) {
    mysqlx::Session& session = database_.getSession();

    session.sql("UPDATE comments "
                "SET content = ? "
                "WHERE id = ?").bind(content, id).execute();
}

void CommentRepository::deleteComment(int id) {
    mysqlx::Session& session = database_.getSession();

    session.sql("DELETE FROM comments "
                "WHERE id = ?").bind(id).execute();
}