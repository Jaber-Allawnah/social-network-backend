#pragma once

#include "../repositories/IPostRepository.hpp"
#include "../repositories/IUserRepository.hpp"
#include "../repositories/IFollowRepository.hpp"

class PostService {
private:
	IPostRepository& postRepository_;
	IFollowRepository& followRepository_;
	IUserRepository& userRepository_;

	void validatePostOwnership(int postId, int requesterId);
	void validateUserPostsAccess(int ownerId, int requesterId);

public:
	PostService(IPostRepository& postRepository,
				IFollowRepository& followRepository,
			    IUserRepository& userRepository);

	Post create(int userId, const std::string& content);
	bool update(int postId, int requesterId, const std::string& content);
	bool remove(int postId, int requesterId);

	Post getPost(int postId, int requesterId);
	std::vector<Post> searchUserPosts(int ownerId, int requesterId, const std::string& keyword);
	std::vector<Post> getUserPosts(int ownerId, int requesterId);
	std::vector<Post> getFeed(int userId);
};