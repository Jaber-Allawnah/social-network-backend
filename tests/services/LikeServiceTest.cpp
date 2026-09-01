#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../../src/services/LikeService.hpp"

#include "../mocks/MockLikeRepository.hpp"
#include "../mocks/MockFollowRepository.hpp"
#include "../mocks/MockPostRepository.hpp"
#include "../mocks/MockUserRepository.hpp"

using ::testing::Return;
using ::testing::_;

class LikeServiceTest : public ::testing::Test {
protected:
    MockLikeRepository mockLikeRepository;
    MockFollowRepository mockFollowRepository;
    MockPostRepository mockPostRepository;
    MockUserRepository mockUserRepository;

    LikeService likeService{mockLikeRepository,
                            mockFollowRepository,
                            mockPostRepository,
                            mockUserRepository};

    User requester{1, "Jaber", "jaber@example.com", "hashed_password"};
    User owner{2, "Raghad", "raghad@example.com", "hashed_password"};

    Post post{10, "Post content", 2};
};

TEST_F(LikeServiceTest, LikePostSuccessfully) {
    EXPECT_CALL(mockPostRepository, getById(10)).WillOnce(Return(post));
    EXPECT_CALL(mockUserRepository, getById(1)).WillOnce(Return(requester));
    EXPECT_CALL(mockFollowRepository, isFollowing(1, 2)).WillOnce(Return(true));
    EXPECT_CALL(mockLikeRepository, hasLiked(1, 10)).WillOnce(Return(false));
    EXPECT_CALL(mockLikeRepository, like(1, 10)).WillOnce(Return(true));

    bool result = likeService.like(1, 10);
    EXPECT_TRUE(result);
}

TEST_F(LikeServiceTest, UnlikePostSuccessfully) {
    EXPECT_CALL(mockPostRepository, getById(10)).WillOnce(Return(post));
    EXPECT_CALL(mockUserRepository, getById(1)).WillOnce(Return(requester));
    EXPECT_CALL(mockFollowRepository, isFollowing(1, 2)).WillOnce(Return(true));
    EXPECT_CALL(mockLikeRepository, hasLiked(1, 10)).WillOnce(Return(true));
    EXPECT_CALL(mockLikeRepository, unlike(1, 10)).WillOnce(Return(true));

    bool result = likeService.unlike(1, 10);
    EXPECT_TRUE(result);
}

TEST_F(LikeServiceTest, LikeThrowsWhenUserAlreadyLikedPost) {
    EXPECT_CALL(mockPostRepository, getById(10)).WillOnce(Return(post));
    EXPECT_CALL(mockUserRepository, getById(1)).WillOnce(Return(requester));
    EXPECT_CALL(mockFollowRepository, isFollowing(1, 2)).WillOnce(Return(true));
    EXPECT_CALL(mockLikeRepository, hasLiked(1, 10)).WillOnce(Return(true));
    EXPECT_CALL(mockLikeRepository, like(_, _)).Times(0);

    EXPECT_THROW(likeService.like(1, 10),std::runtime_error);
}