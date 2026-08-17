#include "./PostRepository.hpp"
#include "../utils/DateTimeUtils.hpp"
#include <mysqlx/xdevapi.h>
#include <ctime>
#include <iomanip>
#include <sstream>

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


std::optional<Post> PostRepository::getById(int id) {
    mysqlx::Session& session = database_.getSession();

    mysqlx::SqlResult result = session.sql("SELECT id, content, user_id, "
                                           "DATE_FORMAT(created_at, '%Y-%m-%d %H:%i:%s'), "
                                           "DATE_FORMAT(updated_at, '%Y-%m-%d %H:%i:%s') "
                                           "FROM posts "
                                           "WHERE id = ?").bind(id).execute();
    mysqlx::Row row = result.fetchOne();

    if (!row)
    {
        return std::nullopt;
    }

    Post post = mapRowToPost(row);

    return post;
}

std::vector<Post> PostRepository::getByUserId(int userId) {
    mysqlx::Session& session = database_.getSession();

    mysqlx::SqlResult result = session.sql("SELECT id, content, user_id, "
                                           "DATE_FORMAT(created_at, '%Y-%m-%d %H:%i:%s'), "
                                           "DATE_FORMAT(updated_at, '%Y-%m-%d %H:%i:%s') "
                                           "FROM posts "
                                           "WHERE user_id = ?").bind(userId).execute();

    auto rows = result.fetchAll();

    std::vector<Post> posts;

    for (const mysqlx::Row& row : rows) {
        posts.push_back(mapRowToPost(row));
    }

    return posts;
}

std::vector<Post> PostRepository::searchUserPosts(int userId, const std::string& keyword) {
    mysqlx::Session& session = database_.getSession();
    std::string formattedKeyword = "%" + keyword + "%";

    mysqlx::SqlResult result = session.sql("SELECT id, content, user_id, "
                                           "DATE_FORMAT(created_at, '%Y-%m-%d %H:%i:%s'), "
                                           "DATE_FORMAT(updated_at, '%Y-%m-%d %H:%i:%s') "
                                           "FROM posts "
                                           "WHERE user_id = ? AND content LIKE ?").bind(userId, formattedKeyword).execute();
    auto rows = result.fetchAll();
    std::vector<Post> userPosts;

    for (const mysqlx::Row& row : rows) {
        userPosts.push_back(mapRowToPost(row));
    }

    return userPosts;
}


void PostRepository::addPost(const std::string& content, int userId) {
    mysqlx::Session& session = database_.getSession();

    session.sql("INSERT INTO posts (content, user_id) "
                "VALUES (?, ?)").bind(content, userId).execute();
}

void PostRepository::updatePost(int id, const std::string& content) {
    mysqlx::Session& session = database_.getSession();

    session.sql("UPDATE posts "
                "SET content = ? "
                "WHERE id = ?").bind(content, id).execute();
}

void PostRepository::deletePost(int id) {
    mysqlx::Session& session = database_.getSession();
    
    session.sql("DELETE FROM posts "
                "WHERE id = ?").bind(id).execute();
}


