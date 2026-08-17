#include "./FollowRequestRepository.hpp"
#include "../utils/DateTimeUtils.hpp"
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>

FollowRequestRepository::FollowRequestRepository(Database& database) : database_(database) {}

FollowRequestStatus FollowRequestRepository::parseStatus(const std::string& status) const {
    if (status == "pending")
        return FollowRequestStatus::Pending;

    if (status == "accepted")
        return FollowRequestStatus::Accepted;

    if (status == "rejected")
        return FollowRequestStatus::Rejected;

    throw std::runtime_error("Invalid follow request status: " + status);
}

FollowRequest FollowRequestRepository::mapRowToFollowRequest(const mysqlx::Row& row) const {
    FollowRequest followRequest;

    followRequest.id = static_cast<int>(row[0]);
    followRequest.requesterId = static_cast<int>(row[1]);
    followRequest.receiverId = static_cast<int>(row[2]);
    followRequest.status = parseStatus(static_cast<std::string>(row[3]));
    followRequest.createdAt = parseDateTime(static_cast<std::string>(row[4]));
    followRequest.updatedAt = parseDateTime(static_cast<std::string>(row[5]));

    return followRequest;
}

std::string FollowRequestRepository::statusToString(FollowRequestStatus status) const {
    if (status == FollowRequestStatus::Pending)
        return "pending";

    if (status == FollowRequestStatus::Accepted)
        return "accepted";

    if (status == FollowRequestStatus::Rejected)
        return "rejected";

    throw std::runtime_error("Invalid follow request status");
}

void FollowRequestRepository::addRequest(int requesterId, int receiverId) {
    mysqlx::Session& session = database_.getSession();

    session.sql("INSERT INTO follow_requests (requester_id, receiver_id) "
                "VALUES (?, ?)").bind(requesterId, receiverId).execute();
}

std::vector<FollowRequest> FollowRequestRepository::getByRequesterId(int requesterId) {
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

std::vector<FollowRequest> FollowRequestRepository::getByReceiverId(int receiverId) {
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

std::optional<FollowRequest> FollowRequestRepository::getById(int id) {
    mysqlx::Session& session = database_.getSession();

    mysqlx::SqlResult result = session.sql("SELECT id, requester_id, receiver_id, status, "
                                           "DATE_FORMAT(created_at, '%Y-%m-%d %H:%i:%s'), "
                                           "DATE_FORMAT(updated_at, '%Y-%m-%d %H:%i:%s') "
                                           "FROM follow_requests "
                                           "WHERE id = ?").bind(id).execute();

    mysqlx::Row row = result.fetchOne();

    if (!row)
        return std::nullopt;

    FollowRequest followRequest = mapRowToFollowRequest(row);
    return followRequest;
}

void FollowRequestRepository::updateStatus(int id, FollowRequestStatus status) {
    mysqlx::Session& session = database_.getSession();

    std::string statusValue = statusToString(status);

    session.sql("UPDATE follow_requests "
                "SET status = ? "
                "WHERE id = ?").bind(statusValue, id).execute();
}