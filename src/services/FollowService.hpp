#pragma once

#include "../repositories/FollowRepository.hpp"
#include "../repositories/FollowRequestRepository.hpp"
#include "../repositories/UserRepository.hpp"
#include "../database/Database.hpp"

class FollowService {
private:
	FollowRepository& followRepository_;
	FollowRequestRepository& followRequestRepository_;
	UserRepository& userRepository_;
	FollowRequest validatePendingRequest(int requestId, int receiverId);
	Database& database_;

public:
	FollowService(FollowRepository& followRepository,
			      FollowRequestRepository& followRequestRepository,
				  UserRepository& userRepository,
				  Database& database);

	bool sendFollowRequest(int requesterId, int receiverId);
	bool acceptFollowRequest(int requestId, int receiverId);
	bool rejectFollowRequest(int requestId, int receiverId);
	bool unfollow(int followerId, int followeeId);

	std::vector<User> getFollowers(int userId);
	std::vector<User> getFollowing(int userId);
};