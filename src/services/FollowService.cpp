#include "FollowService.hpp"

FollowService::FollowService(FollowRepository& followRepository,
							 FollowRequestRepository& followRequestRepository,
							 UserRepository& userRepository) 
						  :  followRepository_(followRepository),
							 followRequestRepository_(followRequestRepository),
							 userRepository_(userRepository) {}

FollowRequest FollowService::validatePendingRequest(int requestId, int receiverId) {
	auto receiver = userRepository_.getById(receiverId);
	if (!receiver) {
		throw std::runtime_error("FollowService: Receiver not found");
	}

	auto followRequest =followRequestRepository_.getById(requestId);
	if (!followRequest) {
		throw std::runtime_error("FollowService: Follow request not found");
	}
	if (followRequest.value().receiverId != receiverId) {
		throw std::runtime_error("FollowService: User not authorized to modify this request");
	}
	if (followRequest.value().status != FollowRequestStatus::Pending) {
		throw std::runtime_error("FollowService: Follow request is not pending");
	}

	return followRequest.value();
}

bool FollowService::sendFollowRequest(int requesterId, int receiverId) {
	if (requesterId == receiverId) {
		throw std::runtime_error("FollowService: User cannot send a follow request to themselves");
	}

	auto requester = userRepository_.getById(requesterId);
	if (!requester) {
		throw std::runtime_error("FollowService: Requester not found");
	}
	auto receiver = userRepository_.getById(receiverId);
	if (!receiver) {
		throw std::runtime_error("FollowService: Receiver not found");
	}

	if (followRepository_.isFollowing(requesterId, receiverId)) {
		throw std::runtime_error("FollowService: User is already following this user");
	}

	auto followRequest = followRequestRepository_.getByUsers(requesterId, receiverId);
	if (followRequest && followRequest.value().status == FollowRequestStatus::Pending) {
		throw std::runtime_error("FollowService: Follow request is already pending");
	}
	else if (followRequest && 
			 (followRequest.value().status == FollowRequestStatus::Rejected ||
		     followRequest.value().status == FollowRequestStatus::Accepted)) {
		return followRequestRepository_.update(followRequest.value().id, 
											   FollowRequestStatus::Pending);
	}
	
	return followRequestRepository_.create(requesterId, receiverId);
}

bool FollowService::acceptFollowRequest(int requestId, int receiverId) {
	FollowRequest followRequest = validatePendingRequest(requestId, receiverId);
	
	followRequestRepository_.update(requestId, FollowRequestStatus::Accepted);
	return followRepository_.follow(followRequest.requesterId, receiverId);
}

bool FollowService::rejectFollowRequest(int requestId, int receiverId) {
	validatePendingRequest(requestId, receiverId);

	return followRequestRepository_.update(requestId, FollowRequestStatus::Rejected);
}

bool FollowService::unfollow(int followerId, int followeeId) {
	auto follower = userRepository_.getById(followerId);
	if (!follower) {
		throw std::runtime_error("FollowService: follower not found");
	}
	auto followee = userRepository_.getById(followeeId);
	if (!followee) {
		throw std::runtime_error("FollowService: Followee not found");
	}

	if (!followRepository_.isFollowing(followerId, followeeId)) {
		throw std::runtime_error("FollowService: User is not following this user");
	}

	return followRepository_.unfollow(followerId, followeeId);
}

std::vector<User> FollowService::getFollowers(int userId) {
	if (!userRepository_.getById(userId)) {
		throw std::runtime_error("FollowService: User not found");
	}

	return followRepository_.getFollowers(userId);
}

std::vector<User> FollowService::getFollowing(int userId) {
	if (!userRepository_.getById(userId)) {
		throw std::runtime_error("FollowService: User not found");
	}

	return followRepository_.getFollowing(userId);
}