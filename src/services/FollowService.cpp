#include "FollowService.hpp"
#include <spdlog/spdlog.h>

FollowService::FollowService(IFollowRepository& followRepository,
							 IFollowRequestRepository& followRequestRepository,
							 IUserRepository& userRepository,
							 ITransactionManager& transactionManager)
						  :  followRepository_(followRepository),
							 followRequestRepository_(followRequestRepository),
							 userRepository_(userRepository),
							 transactionManager_(transactionManager) {}

FollowRequest FollowService::validatePendingRequest(int requestId, int receiverId) {
	auto receiver = userRepository_.getById(receiverId);
	if (!receiver) {
		spdlog::warn("Follow request operation rejected: receiver {} not found", receiverId);
		throw std::runtime_error("FollowService: Receiver not found");
	}

	auto followRequest = followRequestRepository_.getById(requestId);
	if (!followRequest) {
		spdlog::warn("Follow request operation rejected: request {} not found", requestId);
		throw std::runtime_error("FollowService: Follow request not found");
	}

	if (followRequest.value().receiverId != receiverId) {
		spdlog::warn("Follow request operation rejected: user {} is not authorized to modify request {}", receiverId, requestId);
		throw std::runtime_error("FollowService: User not authorized to modify this request");
	}

	if (followRequest.value().status != FollowRequestStatus::Pending) {
		spdlog::warn("Follow request operation rejected: request {} is not pending", requestId);
		throw std::runtime_error("FollowService: Follow request is not pending");
	}

	return followRequest.value();
}

bool FollowService::sendFollowRequest(int requesterId, int receiverId) {
	spdlog::debug("Sending follow request from user {} to user {}", requesterId, receiverId);

	if (requesterId == receiverId) {
		spdlog::warn("Follow request rejected: user {} attempted to follow themselves", requesterId);
		throw std::runtime_error("FollowService: User cannot send a follow request to themselves");
	}

	auto requester = userRepository_.getById(requesterId);
	if (!requester) {
		spdlog::warn("Follow request rejected: requester {} not found", requesterId);
		throw std::runtime_error("FollowService: Requester not found");
	}

	auto receiver = userRepository_.getById(receiverId);
	if (!receiver) {
		spdlog::warn("Follow request rejected: receiver {} not found", receiverId);
		throw std::runtime_error("FollowService: Receiver not found");
	}

	// If the users are currently following each other, do not allow another request.
	if (followRepository_.isFollowing(requesterId, receiverId)) {
		spdlog::warn("Follow request rejected: user {} is already following user {}", requesterId, receiverId);
		throw std::runtime_error("FollowService: User is already following this user");
	}

	// Reuse the existing request row because requester/receiver pairs are unique.
	// Rejected requests can be sent again.
	// An Accepted request reaches this point only if the users are no longer
	// following each other, allowing a new request after an unfollow.
	auto followRequest = followRequestRepository_.getByUsers(requesterId, receiverId);
	if (followRequest && followRequest.value().status == FollowRequestStatus::Pending) {
		spdlog::warn("Follow request rejected: request from user {} to user {} is already pending", requesterId, receiverId);
		throw std::runtime_error("FollowService: Follow request is already pending");
	}
	else if (followRequest &&
			(followRequest.value().status == FollowRequestStatus::Rejected ||
			 followRequest.value().status == FollowRequestStatus::Accepted)) {
		bool updated = followRequestRepository_.update(followRequest.value().id, FollowRequestStatus::Pending);
		spdlog::info("Follow request {} reset-to-pending result from user {} to user {}: {}", 
					 followRequest.value().id, 
					 requesterId, 
					 receiverId, 
					 updated);
		
		return updated;
	}

	bool created = followRequestRepository_.create(requesterId, receiverId);
	spdlog::info("Follow request creation result from user {} to user {}: {}", requesterId, receiverId, created);
	
	return created;
}

bool FollowService::acceptFollowRequest(int requestId, int receiverId) {
	spdlog::debug("Accepting follow request {} by receiver {}", requestId, receiverId);

	FollowRequest followRequest = validatePendingRequest(requestId, receiverId);

	transactionManager_.begin();
	try {
		bool isRequestUpdated = followRequestRepository_.update(requestId, FollowRequestStatus::Accepted);
		if (!isRequestUpdated) {
			throw std::runtime_error("FollowService: Failed to update follow request");
		}

		bool isFollowAdded = followRepository_.follow(followRequest.requesterId, receiverId);
		if (!isFollowAdded) {
			throw std::runtime_error("FollowService: Failed to create follow relationship");
		}

		transactionManager_.commit();
		spdlog::info("Follow request {} accepted by user {}; user {} now follows user {}", requestId, receiverId, followRequest.requesterId, receiverId);
		return true;
	}
	catch (...) {
		transactionManager_.rollback();
		spdlog::error("Failed to accept follow request {}; transaction rolled back", requestId);
		throw;
	}
}

bool FollowService::rejectFollowRequest(int requestId, int receiverId) {
	spdlog::debug("Rejecting follow request {} by receiver {}", requestId, receiverId);

	validatePendingRequest(requestId, receiverId);

	bool rejected = followRequestRepository_.update(requestId, FollowRequestStatus::Rejected);
	spdlog::info("Follow request {} rejection result by user {}: {}", requestId, receiverId, rejected);

	return rejected;
}

bool FollowService::unfollow(int followerId, int followeeId) {
	spdlog::debug("Unfollowing user {} by user {}", followeeId, followerId);

	auto follower = userRepository_.getById(followerId);
	if (!follower) {
		spdlog::warn("Unfollow operation rejected: follower {} not found", followerId);
		throw std::runtime_error("FollowService: follower not found");
	}

	auto followee = userRepository_.getById(followeeId);
	if (!followee) {
		spdlog::warn("Unfollow operation rejected: followee {} not found", followeeId);
		throw std::runtime_error("FollowService: Followee not found");
	}

	if (!followRepository_.isFollowing(followerId, followeeId)) {
		spdlog::warn("Unfollow operation rejected: user {} is not following user {}", followerId, followeeId);
		throw std::runtime_error("FollowService: User is not following this user");
	}

	bool unfollowed = followRepository_.unfollow(followerId, followeeId);
	spdlog::info("Unfollow result for user {} from user {}: {}", followerId, followeeId, unfollowed);

	return unfollowed;
}

std::vector<User> FollowService::getFollowers(int userId) {
	spdlog::debug("Retrieving followers for user {}", userId);

	if (!userRepository_.getById(userId)) {
		spdlog::warn("Followers retrieval rejected: user {} not found", userId);
		throw std::runtime_error("FollowService: User not found");
	}

	std::vector<User> followers = followRepository_.getFollowers(userId);
	spdlog::debug("Retrieved {} followers for user {}", followers.size(), userId);

	return followers;
}

std::vector<User> FollowService::getFollowing(int userId) {
	spdlog::debug("Retrieving following list for user {}", userId);

	if (!userRepository_.getById(userId)) {
		spdlog::warn("Following retrieval rejected: user {} not found", userId);
		throw std::runtime_error("FollowService: User not found");
	}

	std::vector<User> following = followRepository_.getFollowing(userId);
	spdlog::debug("Retrieved {} followed users for user {}", following.size(), userId);

	return following;
}

std::vector<FollowRequest> FollowService::getPendingIncomingRequests(int receiverId) {
	spdlog::debug("Retrieving pending incoming follow requests for user {}", receiverId);

	if (!userRepository_.getById(receiverId)) {
		spdlog::warn("Pending follow requests retrieval rejected: user {} not found", receiverId);
		throw std::runtime_error("FollowService: User not found");
	}

	std::vector<FollowRequest> requests = followRequestRepository_.getUserPendingRequests(receiverId);
	spdlog::debug("Retrieved {} pending incoming follow requests for user {}", requests.size(), receiverId);

	return requests;
}