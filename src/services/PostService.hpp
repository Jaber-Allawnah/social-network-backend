#pragma once

#include "../repositories/PostRepository.hpp"
#include "../repositories/FollowRepository.hpp"
#include "../repositories/UserRepository.hpp"

class PostService {
private:
	PostRepository& postRepository_;
	FollowRepository& followRepository_;
	UserRepository& userRepository_;
	void validatePostOwnership(int postId, int requesterId);
	void validateUserPostsAccess(int ownerId, int requesterId);

public:
	PostService(PostRepository& postRepository, 
				FollowRepository& followRepository, 
				UserRepository& userRepository);

	Post create(int userId, const std::string& content);
	bool update(int postId, int requesterId, const std::string& content);
	bool remove(int postId, int requesterId);

	Post getPost(int postId, int requesterId);
	std::vector<Post> searchUserPosts(int ownerId, int requesterId, const std::string& keyword);
	std::vector<Post> getUserPosts(int ownerId, int requesterId);
	std::vector<Post> getFeed(int userId);
};