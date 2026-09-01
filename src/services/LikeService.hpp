#pragma once

#include "../models/User.hpp"
#include "../repositories/ILikeRepository.hpp"
#include "../repositories/IFollowRepository.hpp"
#include "../repositories/IPostRepository.hpp"
#include "../repositories/IUserRepository.hpp"
#include <vector>

class LikeService {
private:
    ILikeRepository& likeRepository_;
    IFollowRepository& followRepository_;
    IPostRepository& postRepository_;
    IUserRepository& userRepository_;

    void validateUserPostsAccess(int postId, int requesterId);

public:
    LikeService(ILikeRepository& likeRepository, 
                IFollowRepository& followRepository,
                IPostRepository& postRepository, 
                IUserRepository& userRepository);

    bool like(int requesterId, int postId);
    bool unlike(int requesterId, int postId);

    std::vector<User> getUsersWhoLikedPost(int requesterId, int postId);
};