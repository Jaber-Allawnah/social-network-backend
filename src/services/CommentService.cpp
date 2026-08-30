#include "CommentService.hpp"
#include <spdlog/spdlog.h>

CommentService::CommentService(CommentRepository& commentRepository,
							   PostRepository& postRepository,
							   FollowRepository& followRepository,
							   UserRepository& userRepository)
							:  commentRepository_(commentRepository),
							   postRepository_(postRepository),
							   followRepository_(followRepository),
							   userRepository_(userRepository) {}

void CommentService::validateCommentOwnership(int commentId, int userId, int postId) {
	auto comment = commentRepository_.getById(commentId);
	if (!comment) {
		spdlog::warn("Comment operation rejected: comment {} not found", commentId);
		throw std::runtime_error("CommentService: Comment not found");
	}

	if (comment.value().postId != postId) {
		spdlog::warn("Comment operation rejected: comment {} does not belong to post {}", commentId, postId);
		throw std::runtime_error("CommentService: Comment does not belong to this post");
	}

	if (userId != comment.value().userId) {
		spdlog::warn("Comment modification rejected: user {} does not own comment {}", userId, commentId);
		throw std::runtime_error("CommentService: User not authorized to modify this comment");
	}
}

void CommentService::validateUserPostsAccess(int postId, int userId) {
	auto post = postRepository_.getById(postId);
	if (!post) {
		spdlog::warn("Comment operation rejected: post {} not found", postId);
		throw std::runtime_error("CommentService: Post not found");
	}

	int ownerId = post.value().userId;
	if (userId != ownerId && !followRepository_.isFollowing(userId, ownerId)) {
		spdlog::warn("Comment operation rejected: user {} cannot access post {} owned by user {}", userId, postId, ownerId);
		throw std::runtime_error("CommentService: User cannot access this post");
	}
}

void CommentService::validateUserExists(int userId) {
	auto user = userRepository_.getById(userId);
	if (!user) {
		spdlog::warn("Comment operation rejected: user {} not found", userId);
		throw std::runtime_error("CommentService: Requesting user not found");
	}
}

Comment CommentService::create(int userId, int postId, const std::string& content) {
	spdlog::debug("Creating comment for user {} on post {}", userId, postId);

	validateUserExists(userId);
	validateUserPostsAccess(postId, userId);

	if (content.empty()) {
		spdlog::warn("Comment creation rejected: user {} attempted to create an empty comment on post {}", userId, postId);
		throw std::runtime_error("CommentService: Can't add an empty comment");
	}

	Comment comment = commentRepository_.create(userId, postId, content);
	spdlog::info("Comment {} created by user {} on post {}", comment.id, userId, postId);

	return comment;
}

bool CommentService::update(int userId, int commentId, int postId, const std::string& content) {
	spdlog::debug("Updating comment {} by user {} on post {}", commentId, userId, postId);

	validateUserExists(userId);
	validateCommentOwnership(commentId, userId, postId);
	validateUserPostsAccess(postId, userId);

	if (content.empty()) {
		spdlog::warn("Comment update rejected: user {} attempted to update comment {} with empty content", userId, commentId);
		throw std::runtime_error("CommentService: Can't update a comment to an empty comment");
	}

	bool updated = commentRepository_.update(commentId, content);
	if (updated) {
		spdlog::info("Comment {} updated by user {}", commentId, userId);
	}

	return updated;
}

bool CommentService::remove(int userId, int commentId, int postId) {
	spdlog::debug("Removing comment {} by user {} from post {}", commentId, userId, postId);

	validateUserExists(userId);
	validateCommentOwnership(commentId, userId, postId);
	validateUserPostsAccess(postId, userId);

	bool removed = commentRepository_.remove(commentId);
	if (removed) {
		spdlog::info("Comment {} removed by user {}", commentId, userId);
	}

	return removed;
}

std::vector<Comment> CommentService::getPostComments(int postId, int userId) {
	spdlog::debug("Retrieving comments for post {} requested by user {}", postId, userId);

	validateUserExists(userId);
	validateUserPostsAccess(postId, userId);

	std::vector<Comment> comments = commentRepository_.getByPostId(postId);
	spdlog::debug("Retrieved {} comments for post {}", comments.size(), postId);

	return comments;
}