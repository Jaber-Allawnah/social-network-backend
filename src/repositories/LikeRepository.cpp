#include "./LikeRepository.hpp"
#include "../utils/DateTimeUtils.hpp"
#include <ctime>
#include <iomanip>
#include <sstream>

LikeRepository::LikeRepository(Database& database) : database_(database) {}

Like LikeRepository::mapRowToLike(const mysqlx::Row& row) const {
    Like like;

    like.userId = static_cast<int>(row[0]);
    like.postId = static_cast<int>(row[1]);
    like.createdAt = parseDateTime(static_cast<std::string>(row[2]));

    return like;
}

std::vector<Like> LikeRepository::getByPostId(int postId) {
    mysqlx::Session& session = database_.getSession();

    mysqlx::SqlResult result = session.sql("SELECT user_id, post_id, "
                                           "DATE_FORMAT(created_at, '%Y-%m-%d %H:%i:%s') "
                                           "FROM likes "
                                           "WHERE post_id = ?").bind(postId).execute();
    auto rows = result.fetchAll();
    std::vector<Like> postLikes;

    for (const mysqlx::Row& row : rows) {
        postLikes.push_back(mapRowToLike(row));
    }

    return postLikes;
}

void LikeRepository::addLike(int userId, int postId) {
    mysqlx::Session& session = database_.getSession();
    
    session.sql("INSERT INTO likes (user_id, post_id) "
                "VALUES (?, ?)").bind(userId, postId).execute();
}

void LikeRepository::deleteLike(int userId, int postId) {
    mysqlx::Session& session = database_.getSession();

    session.sql("DELETE FROM likes "
                "WHERE post_id = ? AND user_id = ?").bind(postId, userId).execute();
}