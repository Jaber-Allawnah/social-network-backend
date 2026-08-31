#include "LikeService.hpp"
#include <spdlog/spdlog.h>

LikeService::LikeService(LikeRepository& likeRepository,
                         FollowRepository& followRepository,
                         PostRepository& postRepository,
                         UserRepository& userRepository)
                     :   likeRepository_(likeRepository),
                         followRepository_(followRepository),
                         postRepository_(postRepository),
                         userRepository_(userRepository) {}

void LikeService::validateUserPostsAccess(int postId, int requesterId) {
    auto post = postRepository_.getById(postId);
    if (!post) {
        spdlog::warn("Like operation rejected: post {} not found", postId);
        throw std::runtime_error("LikeService: Post not found");
    }

    auto user = userRepository_.getById(requesterId);
    if (!user) {
        spdlog::warn("Like operation rejected: requesting user {} not found", requesterId);
        throw std::runtime_error("LikeService: Requesting user not found");
    }

    int ownerId = post.value().userId;
    if (requesterId != ownerId &&
        !followRepository_.isFollowing(requesterId, ownerId)) {
        spdlog::warn("Like operation rejected: user {} cannot access post {} owned by user {}", requesterId, postId, ownerId);
        throw std::runtime_error("LikeService: User cannot access this post");
    }
}

bool LikeService::like(int requesterId, int postId) {
    spdlog::debug("Liking post {} by user {}", postId, requesterId);

    validateUserPostsAccess(postId, requesterId);

    if (likeRepository_.hasLiked(requesterId, postId)) {
        spdlog::warn("Like operation rejected: user {} has already liked post {}", requesterId, postId);
        throw std::runtime_error("LikeService: User has already liked this post");
    }

    bool liked = likeRepository_.like(requesterId, postId);
    if (liked) {
        spdlog::info("Like result for user {} on post {}: {}", requesterId, postId, liked);
    }

    return liked;
}

bool LikeService::unlike(int requesterId, int postId) {
    spdlog::debug("Unliking post {} by user {}", postId, requesterId);

    validateUserPostsAccess(postId, requesterId);

    if (!likeRepository_.hasLiked(requesterId, postId)) {
        spdlog::warn("Unlike operation rejected: user {} has not liked post {}", requesterId, postId);
        throw std::runtime_error("LikeService: Like does not exist on the post");
    }

    bool unliked = likeRepository_.unlike(requesterId, postId);
    if (unliked) {
        spdlog::info("Unlike result for user {} on post {}: {}", requesterId, postId, unliked);
    }

    return unliked;
}

std::vector<User> LikeService::getUsersWhoLikedPost(int requesterId, int postId) {
    spdlog::debug("Retrieving users who liked post {} requested by user {}", postId, requesterId);

    validateUserPostsAccess(postId, requesterId);

    std::vector<User> users = likeRepository_.getByPostId(postId);
    spdlog::debug("Retrieved {} users who liked post {}", users.size(), postId);

    return users;
}