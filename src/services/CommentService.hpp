#pragma once

#include "../repositories/ICommentRepository.hpp"
#include "../repositories/IPostRepository.hpp"
#include "../repositories/IFollowRepository.hpp"
#include "../repositories/IUserRepository.hpp"

class CommentService {
private:
	ICommentRepository& commentRepository_;
	IPostRepository& postRepository_;
	IFollowRepository& followRepository_;
	IUserRepository& userRepository_;

	void validateCommentOwnership(int commentId, int userId, int postId);
	void validateUserPostsAccess(int postId, int userId);
	void validateUserExists(int userId);

public:
	CommentService(ICommentRepository& commentRepository,
				   IPostRepository& postRepository,
				   IFollowRepository& followRepository,
				   IUserRepository& userRepository);

	Comment create(int userId, int postId, const std::string& content);
	bool update(int userId, int commentId, int postId, const std::string& content);
	bool remove(int userId, int commentId, int postId);

	std::vector<Comment> getPostComments(int postId, int requesterId);
};