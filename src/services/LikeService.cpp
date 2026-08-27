#include "LikeService.hpp"

LikeService::LikeService(LikeRepository& likeRepository,
                         FollowRepository& followRepository,
                         PostRepository& postRepository, 
                         UserRepository& userRepository)
                       : likeRepository_(likeRepository), 
                         followRepository_(followRepository),
                         postRepository_(postRepository),
                         userRepository_(userRepository) {}

void LikeService::validateUserPostsAccess(int postId, int requesterId) {
    auto post = postRepository_.getById(postId);
    if (!post) {
        throw std::runtime_error("LikeService: Post not found");
    }

    auto user = userRepository_.getById(requesterId);
    if (!user) {
        throw std::runtime_error("LikeService: Requesting user not found");
    }

    int ownerId = post.value().userId;
    if (requesterId != ownerId &&
        !followRepository_.isFollowing(requesterId, ownerId)) {
        throw std::runtime_error("LikeService: User cannot access this post");
    }
}

bool LikeService::like(int requesterId, int postId) {
    validateUserPostsAccess(postId, requesterId);

    if (likeRepository_.hasLiked(requesterId, postId)) {
        throw std::runtime_error("LikeService: User has already liked this post");
    }

    return likeRepository_.like(requesterId, postId);
}

bool LikeService::unlike(int requesterId, int postId) {
    validateUserPostsAccess(postId, requesterId);

    if (!likeRepository_.hasLiked(requesterId, postId)) {
        throw std::runtime_error("LikeService: Like does not exist on the post");
    }

    return likeRepository_.unlike(requesterId, postId);
}

std::vector<User> LikeService::getUsersWhoLikedPost(int requesterId, int postId) {
    validateUserPostsAccess(postId, requesterId);

    return likeRepository_.getByPostId(postId);
}