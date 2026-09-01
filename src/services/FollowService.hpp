#pragma once

#include "../repositories/IFollowRepository.hpp"
#include "../repositories/IFollowRequestRepository.hpp"
#include "../repositories/IUserRepository.hpp"
#include "../database/ITransactionManager.hpp"

class FollowService {
private:
	IFollowRepository& followRepository_;
	IFollowRequestRepository& followRequestRepository_;
	IUserRepository& userRepository_;
	ITransactionManager& transactionManager_;

	FollowRequest validatePendingRequest(int requestId, int receiverId);

public:
	FollowService(IFollowRepository& followRepository,
			      IFollowRequestRepository& followRequestRepository,
				  IUserRepository& userRepository,
				  ITransactionManager& transactionManager);

	bool sendFollowRequest(int requesterId, int receiverId);
	bool acceptFollowRequest(int requestId, int receiverId);
	bool rejectFollowRequest(int requestId, int receiverId);
	bool unfollow(int followerId, int followeeId);

	std::vector<FollowRequest> getPendingIncomingRequests(int receiverId);
	std::vector<User> getFollowers(int userId);
	std::vector<User> getFollowing(int userId);
};