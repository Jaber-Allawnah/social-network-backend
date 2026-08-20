#include "CommentService.hpp"

CommentService::CommentService(CommentRepository& commentRepository, 
							   PostRepository& postRepository, 
							   FollowRepository& followRepository,
							   UserRepository& userRepository)
							:  commentRepository_(commentRepository),
							   postRepository_(postRepository),
							   followRepository_(followRepository),
							   userRepository_(userRepository){}

void CommentService::validateCommentOwnership(int commentId, int userId, int postId) {
	auto user = userRepository_.getById(userId);
	if (!user) {
		throw std::runtime_error("CommentService: Requesting user not found");
	}

	auto comment = commentRepository_.getById(commentId);
	if (!comment) {
		throw std::runtime_error("CommentService: Comment not found");
	}
	if (comment.value().postId != postId) {
		throw std::runtime_error("CommentService: Comment does not belong to this post");
	}
	if (userId != comment.value().userId) {
		throw std::runtime_error("CommentService: User not authorized to modify this comment");
	}
}

void CommentService::validatePostAccess(int postId, int userId, bool checkUser) {
	if (checkUser) {
		auto user = userRepository_.getById(userId);
		if (!user) {
			throw std::runtime_error("CommentService: Requesting user not found");
		}
	}

	auto post = postRepository_.getById(postId);
	if (!post) {
		throw std::runtime_error("CommentService: Post not found");
	}
	int ownerId = post.value().userId;
	if (userId != ownerId && !followRepository_.isFollowing(userId, ownerId)) {
		throw std::runtime_error("CommentService: User cannot access this post");
	}
}

Comment CommentService::create(int userId, int postId, const std::string& content) {
	validatePostAccess(postId, userId);
	if (content.empty()) {
		throw std::runtime_error("CommentService: Can't add an empty comment");
	}

	return commentRepository_.create(userId, postId, content);
}

bool CommentService::update(int userId, int commentId, int postId, const std::string& content) {
	validateCommentOwnership(commentId, userId, postId);
	validatePostAccess(postId, userId, false);
	if (content.empty()) {
		throw std::runtime_error("CommentService: Can't update a comment to an empty comment");
	}
	return commentRepository_.update(commentId, content);
}

bool CommentService::remove(int userId, int commentId, int postId) {
	validateCommentOwnership(commentId, userId, postId);
	validatePostAccess(postId, userId, false);

	return commentRepository_.remove(commentId);
}

std::vector<Comment> CommentService::getPostComments(int postId, int userId) {
	validatePostAccess(postId, userId);

	return commentRepository_.getByPostId(postId);
}