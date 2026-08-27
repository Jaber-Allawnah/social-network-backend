#pragma once

#include "../models/User.hpp"
#include "../repositories/LikeRepository.hpp"
#include "../repositories/FollowRepository.hpp"
#include "../repositories/PostRepository.hpp"
#include "../repositories/UserRepository.hpp"
#include <vector>

class LikeService {
private:
    LikeRepository& likeRepository_;
    FollowRepository& followRepository_;
    PostRepository& postRepository_;
    UserRepository& userRepository_;

    void validateUserPostsAccess(int postId, int requesterId);

public:
    LikeService(LikeRepository& likeRepository, 
                FollowRepository& followRepository,
                PostRepository& postRepository, 
                UserRepository& userRepository);

    bool like(int requesterId, int postId);
    bool unlike(int requesterId, int postId);

    std::vector<User> getUsersWhoLikedPost(int requesterId, int postId);
};