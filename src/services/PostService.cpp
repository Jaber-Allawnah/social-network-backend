#include "PostService.hpp"
#include <stdexcept>
#include <spdlog/spdlog.h>

PostService::PostService(PostRepository& postRepository,
						 FollowRepository& followRepository,
						 UserRepository& userRepository)
					:    postRepository_(postRepository),
						 followRepository_(followRepository),
						 userRepository_(userRepository) {
}

void PostService::validatePostOwnership(int postId, int requesterId) {
	auto user = userRepository_.getById(requesterId);
	if (!user) {
		spdlog::warn("Post operation rejected: requesting user {} not found", requesterId);
		throw std::runtime_error("PostService: Requesting user not found");
	}

	auto post = postRepository_.getById(postId);
	if (!post) {
		spdlog::warn("Post operation rejected: post {} not found", postId);
		throw std::runtime_error("PostService: Post not found ");
	}

	if (post.value().userId != requesterId) {
		spdlog::warn("Post modification rejected: user {} does not own post {}", requesterId, postId);
		throw std::runtime_error("PostService: User not authorized to modify this post ");
	}
}

void PostService::validateUserPostsAccess(int ownerId, int requesterId) {
	auto user = userRepository_.getById(requesterId);
	if (!user) {
		spdlog::warn("Post access rejected: requesting user {} not found", requesterId);
		throw std::runtime_error("PostService: Requesting user not found");
	}

	auto owner = userRepository_.getById(ownerId);
	if (!owner) {
		spdlog::warn("Post access rejected: post owner {} not found", ownerId);
		throw std::runtime_error("PostService: Post owner not found");
	}

	if (ownerId != requesterId && !followRepository_.isFollowing(requesterId, ownerId)) {
		spdlog::warn("Post access rejected: user {} cannot access posts owned by user {}", requesterId, ownerId);
		throw std::runtime_error("PostService: Can't retrieve the posts of someone you don't follow ");
	}
}

Post PostService::create(int userId, const std::string& content) {
	spdlog::debug("Creating post for user {}", userId);

	auto user = userRepository_.getById(userId);
	if (!user) {
		spdlog::warn("Post creation rejected: user {} not found", userId);
		throw std::runtime_error("PostService: Requesting user not found");
	}

	if (content.empty()) {
		spdlog::warn("Post creation rejected: user {} attempted to create an empty post", userId);
		throw std::runtime_error("PostService: Can't add an empty post");
	}

	Post post = postRepository_.create(userId, content);
	spdlog::info("Post {} created by user {}", post.id, userId);

	return post;
}

bool PostService::update(int postId, int requesterId, const std::string& content) {
	spdlog::debug("Updating post {} by user {}", postId, requesterId);

	validatePostOwnership(postId, requesterId);

	if (content.empty()) {
		spdlog::warn("Post update rejected: user {} attempted to update post {} with empty content", requesterId, postId);
		throw std::runtime_error("PostService: Can't update post with empty content");
	}

	bool updated = postRepository_.update(postId, content);
	spdlog::info("Post {} update result by user {}: {}", postId, requesterId, updated);

	return updated;
}

bool PostService::remove(int postId, int requesterId) {
	spdlog::debug("Removing post {} by user {}", postId, requesterId);

	validatePostOwnership(postId, requesterId);

	bool removed = postRepository_.remove(postId);
	spdlog::info("Post {} removal result by user {}: {}", postId, requesterId, removed);
	
	return removed;
}

Post PostService::getPost(int postId, int requesterId) {
	spdlog::debug("Retrieving post {} requested by user {}", postId, requesterId);

	auto post = postRepository_.getById(postId);
	if (!post) {
		spdlog::warn("Post retrieval rejected: post {} not found", postId);
		throw std::runtime_error("PostService: Can't retrieve a non-existing post");
	}

	validateUserPostsAccess(post.value().userId, requesterId);

	spdlog::debug("Post {} retrieved for user {}", postId, requesterId);

	return post.value();
}

std::vector<Post> PostService::searchUserPosts(int ownerId, int requesterId, const std::string& keyword) {
	spdlog::debug("Searching posts owned by user {} requested by user {}", ownerId, requesterId);

	validateUserPostsAccess(ownerId, requesterId);

	if (keyword.empty()) {
		spdlog::warn("Post search rejected: user {} attempted to search posts of user {} with an empty keyword", requesterId, ownerId);
		throw std::runtime_error("PostService: Can't search using an empty keyword");
	}

	std::vector<Post> posts = postRepository_.searchUserPosts(ownerId, keyword);
	spdlog::debug("Retrieved {} matching posts owned by user {}", posts.size(), ownerId);

	return posts;
}

std::vector<Post> PostService::getUserPosts(int ownerId, int requesterId) {
	spdlog::debug("Retrieving posts owned by user {} requested by user {}", ownerId, requesterId);

	validateUserPostsAccess(ownerId, requesterId);

	std::vector<Post> posts = postRepository_.getByUserId(ownerId);
	spdlog::debug("Retrieved {} posts owned by user {}", posts.size(), ownerId);

	return posts;
}

std::vector<Post> PostService::getFeed(int userId) {
	spdlog::debug("Retrieving feed for user {}", userId);

	auto user = userRepository_.getById(userId);
	if (!user) {
		spdlog::warn("Feed retrieval rejected: user {} not found", userId);
		throw std::runtime_error("PostService: Requesting user not found");
	}

	std::vector<Post> posts = postRepository_.getUserFeed(userId);
	spdlog::debug("Retrieved {} posts for user {} feed", posts.size(), userId);

	return posts;
}