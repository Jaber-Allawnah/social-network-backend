#include "./FollowRequestRepository.hpp"
#include "../utils/DateTimeUtils.hpp"
#include "../utils/FollowRequestStatusUtils.hpp"
#include <stdexcept>
#include <spdlog/spdlog.h>

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
    spdlog::debug("Creating follow request from user {} to user {} in database", requesterId, receiverId);

    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            INSERT INTO follow_requests (requester_id, receiver_id)
            VALUES (?, ?))";
        mysqlx::SqlResult result = session.sql(sql).bind(requesterId, receiverId).execute();

        bool created = result.getAffectedItemsCount() > 0;
        spdlog::debug("Follow request creation result from user {} to user {}: {}", requesterId, receiverId, created);

        return created;
    }
    catch (const mysqlx::Error& error) {
        spdlog::error("Failed to create follow request from user {} to user {} in database: {}", 
                      requesterId, 
                      receiverId, 
                      error.what());
        throw std::runtime_error("FollowRequestRepository: Failed to create follow request: " + std::string(error.what()));
    }
}

std::vector<FollowRequest> FollowRequestRepository::getByRequesterId(int requesterId) {
    spdlog::debug("Retrieving follow requests sent by user {} from database", requesterId);

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

        spdlog::debug("Retrieved {} follow requests sent by user {} from database", followRequests.size(), requesterId);

        return followRequests;
    }
    catch (const mysqlx::Error& error) {
        spdlog::error("Failed to retrieve follow requests sent by user {} from database: {}", requesterId, error.what());
        throw std::runtime_error("FollowRequestRepository: Failed to retrieve follow requests by requester id: " + 
                                 std::string(error.what()));
    }
}

std::vector<FollowRequest> FollowRequestRepository::getByReceiverId(int receiverId) {
    spdlog::debug("Retrieving follow requests received by user {} from database", receiverId);

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

        spdlog::debug("Retrieved {} follow requests received by user {} from database", followRequests.size(), receiverId);

        return followRequests;
    }
    catch (const mysqlx::Error& error) {
        spdlog::error("Failed to retrieve follow requests received by user {} from database: {}", receiverId, error.what());
        throw std::runtime_error("FollowRequestRepository: Failed to retrieve follow requests by receiver id: " + 
                                 std::string(error.what()));
    }
}

std::optional<FollowRequest> FollowRequestRepository::getById(int followRequestId) {
    spdlog::debug("Retrieving follow request {} from database", followRequestId);

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
        if (!row) {
            spdlog::debug("Follow request {} not found in database", followRequestId);
            return std::nullopt;
        }

        FollowRequest followRequest = mapRowToFollowRequest(row);

        spdlog::debug("Follow request {} retrieved from database", followRequestId);

        return followRequest;
    }
    catch (const mysqlx::Error& error) {
        spdlog::error("Failed to retrieve follow request {} from database: {}", followRequestId, error.what());
        throw std::runtime_error("FollowRequestRepository: Failed to retrieve follow request by id: " + 
                                 std::string(error.what()));
    }
}

bool FollowRequestRepository::update(int followRequestId, FollowRequestStatus status) {
    std::string statusValue = followRequestStatusToString(status);
    spdlog::debug("Updating follow request {} status to {} in database", followRequestId, statusValue);

    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            UPDATE follow_requests
            SET status = ?
            WHERE id = ?)";
        mysqlx::SqlResult result = session.sql(sql).bind(statusValue, followRequestId).execute();

        bool updated = result.getAffectedItemsCount() > 0;
        spdlog::debug("Follow request {} status update result: {}", followRequestId, updated);

        return updated;
    }
    catch (const mysqlx::Error& error) {
        spdlog::error("Failed to update follow request {} status to {} in database: {}", 
                      followRequestId, 
                      statusValue, 
                      error.what());
        throw std::runtime_error("FollowRequestRepository: Failed to update follow request status: " + 
                                 std::string(error.what()));
    }
}

std::optional<FollowRequest> FollowRequestRepository::getByUsers(int requesterId, int receiverId) {
    spdlog::debug("Retrieving follow request from user {} to user {} from database", requesterId, receiverId);

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
            spdlog::debug("No follow request found from user {} to user {}", requesterId, receiverId);
            return std::nullopt;
        }

        FollowRequest followRequest = mapRowToFollowRequest(row);
        spdlog::debug("Follow request {} retrieved from user {} to user {}", followRequest.id, requesterId, receiverId);

        return followRequest;
    }
    catch (const mysqlx::Error& error) {
        spdlog::error("Failed to retrieve follow request from user {} to user {} from database: {}", 
                      requesterId, 
                      receiverId, 
                      error.what());
        throw std::runtime_error("FollowRequestRepository: Failed to retrieve follow request: " +
            std::string(error.what()));
    }
}

std::vector<FollowRequest> FollowRequestRepository::getUserPendingRequests(int userId) {
    spdlog::debug("Retrieving pending follow requests for user {} from database", userId);

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
            WHERE receiver_id = ? AND status = 'pending')";

        mysqlx::SqlResult result = session.sql(sql).bind(userId).execute();
        auto rows = result.fetchAll();
        std::vector<FollowRequest> pendingRequests;
        for (const mysqlx::Row& row : rows) {
            pendingRequests.push_back(mapRowToFollowRequest(row));
        }

        spdlog::debug("Retrieved {} pending follow requests for user {} from database", pendingRequests.size(), userId);

        return pendingRequests;
    }
    catch (const mysqlx::Error& error) {
        spdlog::error("Failed to retrieve pending follow requests for user {} from database: {}", userId, error.what());
        throw std::runtime_error("FollowRequestRepository: Failed to retrieve pending follow requests: " +
            std::string(error.what()));
    }
}