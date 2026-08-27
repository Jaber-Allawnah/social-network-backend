#include "./FollowRequestRepository.hpp"
#include "../utils/DateTimeUtils.hpp"
#include "../utils/FollowRequestStatusUtils.hpp"
#include <stdexcept>

using namespace DateTimeUtils;
using namespace FollowRequestStatusUtils;

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

bool FollowRequestRepository::create(int requesterId, int receiverId) {
    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            INSERT INTO follow_requests (requester_id, receiver_id)
            VALUES (?, ?))";
        mysqlx::SqlResult result = session.sql(sql).bind(requesterId, receiverId).execute();

        return result.getAffectedItemsCount() > 0;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("FollowRequestRepository: Failed to create follow request: " + std::string(error.what()));
    }
}

std::vector<FollowRequest> FollowRequestRepository::getByRequesterId(int requesterId) {
    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            SELECT
                id,
                requester_id,
                receiver_id,
                status,
                DATE_FORMAT(created_at, '%Y-%m-%d %H:%i:%s'),
                DATE_FORMAT(updated_at, '%Y-%m-%d %H:%i:%s')
            FROM follow_requests
            WHERE requester_id = ?)";
        mysqlx::SqlResult result = session.sql(sql).bind(requesterId).execute();
        auto rows = result.fetchAll();
        std::vector<FollowRequest> followRequests;
        for (const mysqlx::Row& row : rows) {
            followRequests.push_back(mapRowToFollowRequest(row));
        }

        return followRequests;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("FollowRequestRepository: Failed to retrieve follow requests by requester id: " + std::string(error.what()));
    }
}

std::vector<FollowRequest> FollowRequestRepository::getByReceiverId(int receiverId) {
    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            SELECT
                id,
                requester_id,
                receiver_id,
                status,
                DATE_FORMAT(created_at, '%Y-%m-%d %H:%i:%s'),
                DATE_FORMAT(updated_at, '%Y-%m-%d %H:%i:%s')
            FROM follow_requests
            WHERE receiver_id = ?)";
        mysqlx::SqlResult result = session.sql(sql).bind(receiverId).execute();
        auto rows = result.fetchAll();
        std::vector<FollowRequest> followRequests;
        for (const mysqlx::Row& row : rows) {
            followRequests.push_back(mapRowToFollowRequest(row));
        }

        return followRequests;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("FollowRequestRepository: Failed to retrieve follow requests by receiver id: " + std::string(error.what()));
    }
}

std::optional<FollowRequest> FollowRequestRepository::getById(int followRequestId) {
    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            SELECT
                id,
                requester_id,
                receiver_id,
                status,
                DATE_FORMAT(created_at, '%Y-%m-%d %H:%i:%s'),
                DATE_FORMAT(updated_at, '%Y-%m-%d %H:%i:%s')
            FROM follow_requests
            WHERE id = ?)";
        mysqlx::SqlResult result = session.sql(sql).bind(followRequestId).execute();
        mysqlx::Row row = result.fetchOne();
        if (!row)
            return std::nullopt;

        FollowRequest followRequest = mapRowToFollowRequest(row);

        return followRequest;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("FollowRequestRepository: Failed to retrieve follow request by id: " + std::string(error.what()));
    }
}

bool FollowRequestRepository::update(int followRequestId, FollowRequestStatus status) {
    try {
        mysqlx::Session& session = database_.getSession();
        std::string statusValue = followRequestStatusToString(status);
        const std::string sql = R"(
            UPDATE follow_requests
            SET status = ?
            WHERE id = ?)";
        mysqlx::SqlResult result = session.sql(sql).bind(statusValue, followRequestId).execute();

        return result.getAffectedItemsCount() > 0;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("FollowRequestRepository: Failed to update follow request status: " + std::string(error.what()));
    }
}

std::optional<FollowRequest> FollowRequestRepository::getByUsers(int requesterId, int receiverId) {
    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            SELECT
                id,
                requester_id,
                receiver_id,
                status,
                DATE_FORMAT(created_at, '%Y-%m-%d %H:%i:%s'),
                DATE_FORMAT(updated_at, '%Y-%m-%d %H:%i:%s')
            FROM follow_requests
            WHERE requester_id = ?
               AND receiver_id = ?)";

        mysqlx::SqlResult result = session.sql(sql).bind(requesterId, receiverId).execute();
        mysqlx::Row row = result.fetchOne();
        if (!row) {
            return std::nullopt;
        }

        return mapRowToFollowRequest(row);
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("FollowRequestRepository: Failed to retrieve follow request: " +
            std::string(error.what()));
    }
}