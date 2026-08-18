#pragma once

#include "../database/Database.hpp"
#include "../models/FollowRequest.hpp"
#include <string>
#include <mysqlx/xdevapi.h>
#include <vector>
#include <optional>

class FollowRequestRepository {
private:
	Database& database_;
	FollowRequest mapRowToFollowRequest(const mysqlx::Row& row) const;

public:
	FollowRequestRepository(Database& database);

	void create(int requesterId, int receiverId);

	std::vector<FollowRequest> getByRequesterId(int requesterId);
	std::vector<FollowRequest> getByReceiverId(int receiverId);
	std::optional<FollowRequest> getById(int followRequestId);

	bool update(int followRequestId, FollowRequestStatus status);
};