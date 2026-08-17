#include "./FollowRepository.hpp"
#include "../utils/DateTimeUtils.hpp"

FollowRepository::FollowRepository(Database& datebase) : database_(datebase) {}

void FollowRepository::addFollow(int followerId, int followeeId) {
    mysqlx::Session& session = database_.getSession();

    session.sql("INSERT INTO follows (follower_id, followee_id) "
                "VALUES (?, ?)").bind(followerId, followeeId).execute();
}

void FollowRepository::deleteFollow(int followerId, int followeeId) {
    mysqlx::Session& session = database_.getSession();

    session.sql("DELETE FROM follows "
                "WHERE follower_id = ? AND followee_id = ?").bind(followerId, followeeId).execute();
}

std::vector<int> FollowRepository::getFollowerIds(int userId) {
    mysqlx::Session& session = database_.getSession();
    
    mysqlx::SqlResult result = session.sql("SELECT follower_id "
                                           "FROM follows "
                                           "WHERE followee_id = ?").bind(userId).execute();

    auto rows = result.fetchAll();
    std::vector<int> followersIds;

    for (const mysqlx::Row& row : rows) {
        followersIds.push_back(static_cast<int>(row[0]));
    }

    return followersIds;
}

std::vector<int> FollowRepository::getFollowingIds(int userId) {
    mysqlx::Session& session = database_.getSession();

    mysqlx::SqlResult result = session.sql("SELECT followee_id "
                                           "FROM follows "
                                           "WHERE follower_id = ?").bind(userId).execute();

    auto rows = result.fetchAll();
    std::vector<int> followeeIds;

    for (const mysqlx::Row& row : rows) {
        followeeIds.push_back(static_cast<int>(row[0]));
    }

    return followeeIds;
}
