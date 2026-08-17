#pragma once

#include "./FollowRequestStatus.hpp"
#include <chrono>
struct FollowRequest {
	int id;
	int requesterId;
	int receiverId;
	FollowRequestStatus status;
	std::chrono::system_clock::time_point createdAt;
	std::chrono::system_clock::time_point updatedAt;
};