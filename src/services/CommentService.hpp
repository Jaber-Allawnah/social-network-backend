#pragma once

#include "../repositories/CommentRepository.hpp"
#include "../repositories/PostRepository.hpp"
#include "../repositories/FollowRepository.hpp"
#include "../repositories/UserRepository.hpp"

class CommentService {
private:
	CommentRepository& commentRepository_;
	PostRepository& postRepository_;
	FollowRepository& followRepository_;
	UserRepository& userRepository_;
	void validateCommentOwnership(int commentId, int userId, int postId);
	void validatePostAccess(int postId, int userId, bool checkUser = true);

public:
	CommentService(CommentRepository& commentRepository, 
		   		   PostRepository& postRepository, 
				   FollowRepository& followRepository,
				   UserRepository& userRepository);

	Comment create(int userId, int postId, const std::string& content);
	bool update(int userId, int commentId, int postId, const std::string& content);
	bool remove(int userId, int commentId, int postId);

	std::vector<Comment> getPostComments(int postId, int requesterId);
};