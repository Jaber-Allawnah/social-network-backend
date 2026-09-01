#pragma once

#include "../database/Database.hpp"
#include "../models/FollowRequest.hpp"
#include "./IFollowRequestRepository.hpp"
#include <string>
#include <mysqlx/xdevapi.h>
#include <vector>
#include <optional>

class FollowRequestRepository : public IFollowRequestRepository {
private:
	Database& database_;

	FollowRequest mapRowToFollowRequest(const mysqlx::Row& row) const;

public:
	FollowRequestRepository(Database& database);

	bool create(int requesterId, int receiverId) override;
	bool update(int followRequestId, FollowRequestStatus status) override;

	std::vector<FollowRequest> getByRequesterId(int requesterId) override;
	std::vector<FollowRequest> getByReceiverId(int receiverId) override;
	std::vector<FollowRequest> getUserPendingRequests(int userId) override;
	std::optional<FollowRequest> getById(int followRequestId) override;
	std::optional<FollowRequest> getByUsers(int requesterId, int receiverId) override;


};