#pragma once

#include "../database/Database.hpp"
#include "../models/FollowRequest.hpp"
#include <string>
#include <mysqlx/xdevapi.h>
#include <vector>
#include <chrono>
#include <optional>

class FollowRequestRepository {
private:
	Database& database_;

	FollowRequestStatus parseStatus(const std::string& status) const;

	FollowRequest mapRowToFollowRequest(const mysqlx::Row& row) const;

	std::string statusToString(FollowRequestStatus status) const;

public:
	FollowRequestRepository(Database& database);

	// Add Operation
	void addRequest(int requesterId, int receiverId);

	// Get Operations
	std::vector<FollowRequest> getByRequesterId(int requesterId);
	std::vector<FollowRequest> getByReceiverId(int receiverId);
	std::optional<FollowRequest> getById(int id);

	// Update Operation
	void updateStatus(int id, FollowRequestStatus status);
};