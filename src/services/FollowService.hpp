#pragma once

#include "../repositories/FollowRepository.hpp"
#include "../repositories/FollowRequestRepository.hpp"
#include "../repositories/UserRepository.hpp"

class FollowService {
private:
	FollowRepository& followRepository_;
	FollowRequestRepository& followRequestRepository_;
	UserRepository& userRepository_;
	FollowRequest validatePendingRequest(int requestId, int receiverId);

public:
	FollowService(FollowRepository& followRepository,
			      FollowRequestRepository& followRequestRepository,
				  UserRepository& userRepository);

	bool sendFollowRequest(int requesterId, int receiverId);
	bool acceptFollowRequest(int requestId, int receiverId);
	bool rejectFollowRequest(int requestId, int receiverId);
	bool unfollow(int followerId, int followeeId);

	std::vector<User> getFollowers(int userId);
	std::vector<User> getFollowing(int userId);
};