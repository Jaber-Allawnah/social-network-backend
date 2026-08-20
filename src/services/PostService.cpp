#include "PostService.hpp"
#include <stdexcept>

PostService::PostService(PostRepository& postRepository, 
						 FollowRepository& followRepository,
						 UserRepository& userRepository)
				       : postRepository_(postRepository),
						 followRepository_(followRepository),
						 userRepository_(userRepository) {}

void PostService::validatePostOwnership(int postId, int requesterId) {
	auto user = userRepository_.getById(requesterId);
	if (!user) {
		throw std::runtime_error("PostService: Requesting user not found");
	}
	auto post = postRepository_.getById(postId);
	if (!post) {
		throw std::runtime_error("PostService: Post not found ");
	}
	if (post.value().userId != requesterId) {
		throw std::runtime_error("PostService: User not authorized to modify this post ");
	}
}

void PostService::validatePostAccess(int ownerId, int requesterId) {
	auto user = userRepository_.getById(requesterId);
	if (!user) {
		throw std::runtime_error("PostService: Requesting user not found");
	}
	auto owner = userRepository_.getById(ownerId);
	if (!owner) {
		throw std::runtime_error("PostService: Post owner not found");
	}

	if (ownerId != requesterId && !followRepository_.isFollowing(requesterId, ownerId)) {
		throw std::runtime_error("PostService: Can't retrieve the posts of someone you don't follow ");
	}
}

Post PostService::create(int userId, const std::string& content) {
	auto user = userRepository_.getById(userId);
	if (!user) {
		throw std::runtime_error("PostService: Requesting user not found");
	}
	return postRepository_.create(userId, content);
}

bool PostService::update(int postId, int requesterId, const std::string& content) {
	validatePostOwnership(postId, requesterId);
	return postRepository_.update(postId, content);
}

bool PostService::remove(int postId, int requesterId) {
	validatePostOwnership(postId, requesterId);
	return postRepository_.remove(postId);
}

Post PostService::getPost(int postId, int requesterId) {
	auto post = postRepository_.getById(postId);
	if (!post) {
		throw std::runtime_error("PostService: Can't retrieve a non-existing post ");
	}
	validatePostAccess(post.value().userId, requesterId);

	return post.value();
}

std::vector<Post> PostService::searchUserPosts(int ownerId, int requesterId, const std::string& keyword) {
	validatePostAccess(ownerId, requesterId);
	return postRepository_.searchUserPosts(ownerId, keyword);
}

std::vector<Post> PostService::getUserPosts(int ownerId, int requesterId) {
	validatePostAccess(ownerId, requesterId);
	return postRepository_.getByUserId(ownerId);
}

std::vector<Post> PostService::getFeed(int userId) {
	auto user = userRepository_.getById(userId);
	if (!user) {
		throw std::runtime_error("PostService: Requesting user not found");
	}
	std::vector<User> following = followRepository_.getFollowing(userId);
	std::vector<Post> feed;
	for (const User& user : following) {
		std::vector<Post> userPosts = postRepository_.getByUserId(user.id);
		feed.insert(feed.end(), userPosts.begin(), userPosts.end());
	}

	return feed;
}