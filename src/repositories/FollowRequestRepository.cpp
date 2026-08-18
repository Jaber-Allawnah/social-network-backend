#include "./FollowRequestRepository.hpp"
#include "../utils/DateTimeUtils.hpp"
#include "../utils/FollowRequestStatusUtils.hpp"
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>

FollowRequestRepository::FollowRequestRepository(Database& database) : database_(database) {}

FollowRequest FollowRequestRepository::mapRowToFollowRequest(const mysqlx::Row& row) const {
    FollowRequest followRequest;
    followRequest.id = static_cast<int>(row[0]);
    followRequest.requesterId = static_cast<int>(row[1]);
    followRequest.receiverId = static_cast<int>(row[2]);
    followRequest.status = parseFollowRequestStatus(static_cast<std::string>(row[3]));
    followRequest.createdAt = parseDateTime(static_cast<std::string>(row[4]));
    followRequest.updatedAt = parseDateTime(static_cast<std::string>(row[5]));

    return followRequest;
}

void FollowRequestRepository::create(int requesterId, int receiverId) {
    try {
        mysqlx::Session& session = database_.getSession();
        session.sql("INSERT INTO follow_requests (requester_id, receiver_id) "
                    "VALUES (?, ?)").bind(requesterId, receiverId).execute();
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("Failed to create follow request " + std::string(error.what()));
    }
}

std::vector<FollowRequest> FollowRequestRepository::getByRequesterId(int requesterId) {
    try {
        mysqlx::Session& session = database_.getSession();
        mysqlx::SqlResult result = session.sql("SELECT id, requester_id, receiver_id, status, "
                                               "DATE_FORMAT(created_at, '%Y-%m-%d %H:%i:%s'), "
                                               "DATE_FORMAT(updated_at, '%Y-%m-%d %H:%i:%s') "
                                               "FROM follow_requests "
                                               "WHERE requester_id = ?").bind(requesterId).execute();
        auto rows = result.fetchAll();
        std::vector<FollowRequest> followRequests;
        for (const mysqlx::Row& row : rows) {
            followRequests.push_back(mapRowToFollowRequest(row));
        }

        return followRequests;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("Failed to retrieve follow requests by requester id " + std::string(error.what()));
    }
}

std::vector<FollowRequest> FollowRequestRepository::getByReceiverId(int receiverId) {
    try {
        mysqlx::Session& session = database_.getSession();
        mysqlx::SqlResult result = session.sql("SELECT id, requester_id, receiver_id, status, "
                                               "DATE_FORMAT(created_at, '%Y-%m-%d %H:%i:%s'), "
                                               "DATE_FORMAT(updated_at, '%Y-%m-%d %H:%i:%s') "
                                               "FROM follow_requests "
                                               "WHERE receiver_id = ?").bind(receiverId).execute();
        auto rows = result.fetchAll();
        std::vector<FollowRequest> followRequests;
        for (const mysqlx::Row& row : rows) {
            followRequests.push_back(mapRowToFollowRequest(row));
        }

        return followRequests;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("Failed to retrieve follow requests by receiver id " + std::string(error.what()));
    }
}

std::optional<FollowRequest> FollowRequestRepository::getById(int followRequestId) {
    try {
        mysqlx::Session& session = database_.getSession();
        mysqlx::SqlResult result = session.sql("SELECT id, requester_id, receiver_id, status, "
                                               "DATE_FORMAT(created_at, '%Y-%m-%d %H:%i:%s'), "
                                               "DATE_FORMAT(updated_at, '%Y-%m-%d %H:%i:%s') "
                                               "FROM follow_requests "
                                               "WHERE id = ?").bind(followRequestId).execute();
        mysqlx::Row row = result.fetchOne();
        if (!row)
            return std::nullopt;

        FollowRequest followRequest = mapRowToFollowRequest(row);

        return followRequest;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("Failed to retrieve follow request by id " + std::string(error.what()));
    }
}

bool FollowRequestRepository::update(int followRequestId, FollowRequestStatus status) {
    try {
         mysqlx::Session& session = database_.getSession();
         std::string statusValue = followRequestStatusToString(status);
         mysqlx::SqlResult result = session.sql("UPDATE follow_requests "
                                                "SET status = ? "
                                                "WHERE id = ?").bind(statusValue, followRequestId).execute();

         return result.getAffectedItemsCount() > 0;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("Failed to update follow request status " + std::string(error.what()));
    }
}