#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../../src/services/CommentService.hpp"

#include "../mocks/MockCommentRepository.hpp"
#include "../mocks/MockPostRepository.hpp"
#include "../mocks/MockFollowRepository.hpp"
#include "../mocks/MockUserRepository.hpp"

using ::testing::Return;
using ::testing::_;

class CommentServiceTest : public ::testing::Test {
protected:
    MockCommentRepository mockCommentRepository;
    MockPostRepository mockPostRepository;
    MockFollowRepository mockFollowRepository;
    MockUserRepository mockUserRepository;

    CommentService commentService{
        mockCommentRepository,
        mockPostRepository,
        mockFollowRepository,
        mockUserRepository
    };

    User user{1, "Jaber", "jaber@example.com", "hashed_password"};
    User owner{2, "Raghad", "raghad@example.com", "hashed_password"};

    Post post{10, "Post content", 2};

    Comment comment{20, "Old comment", 1, 10};
};

TEST_F(CommentServiceTest, CreateCommentSuccessfully) {
    EXPECT_CALL(mockUserRepository, getById(1)).WillOnce(Return(user));
    EXPECT_CALL(mockPostRepository, getById(10)).WillOnce(Return(post));
    EXPECT_CALL(mockFollowRepository, isFollowing(1, 2)).WillOnce(Return(true));
    EXPECT_CALL(mockCommentRepository, create(1, 10, "Hello")).WillOnce(Return(Comment{ 20, "Hello", 1, 10 }));

    Comment result = commentService.create(1, 10, "Hello");
    EXPECT_EQ(result.id, 20);
    EXPECT_EQ(result.userId, 1);
    EXPECT_EQ(result.postId, 10);
    EXPECT_EQ(result.content, "Hello");
}

TEST_F(CommentServiceTest, UpdateCommentSuccessfully) {
    EXPECT_CALL(mockUserRepository, getById(1)).WillOnce(Return(user));
    EXPECT_CALL(mockCommentRepository, getById(20)).WillOnce(Return(comment));
    EXPECT_CALL(mockPostRepository, getById(10)).WillOnce(Return(post));
    EXPECT_CALL(mockFollowRepository, isFollowing(1, 2)).WillOnce(Return(true));
    EXPECT_CALL(mockCommentRepository, update(20, "New comment")).WillOnce(Return(true));

    bool result = commentService.update(1, 20, 10, "New comment");
    EXPECT_TRUE(result);
}

TEST_F(CommentServiceTest, RemoveCommentSuccessfully) {
    EXPECT_CALL(mockUserRepository, getById(1)).WillOnce(Return(user));
    EXPECT_CALL(mockCommentRepository, getById(20)).WillOnce(Return(comment));
    EXPECT_CALL(mockPostRepository, getById(10)).WillOnce(Return(post));
    EXPECT_CALL(mockFollowRepository, isFollowing(1, 2)).WillOnce(Return(true));
    EXPECT_CALL(mockCommentRepository, remove(20)).WillOnce(Return(true));

    bool result = commentService.remove(1, 20, 10);
    EXPECT_TRUE(result);
}

TEST_F(CommentServiceTest, UpdateThrowsWhenUserDoesNotOwnComment) {
    Comment anotherUsersComment{20, "Old comment", 2, 10};

    EXPECT_CALL(mockUserRepository, getById(1)).WillOnce(Return(user));
    EXPECT_CALL(mockCommentRepository, getById(20)).WillOnce(Return(anotherUsersComment));
    EXPECT_CALL(mockCommentRepository, update(_, _)).Times(0);
    EXPECT_THROW(commentService.update(1, 20, 10, "New comment"),std::runtime_error);
}

TEST_F(CommentServiceTest, RemoveThrowsWhenUserDoesNotOwnComment) {
    Comment anotherUsersComment{ 20, "Old comment", 2, 10 };

    EXPECT_CALL(mockUserRepository, getById(1)).WillOnce(Return(user));
    EXPECT_CALL(mockCommentRepository, getById(20)).WillOnce(Return(anotherUsersComment));
    EXPECT_CALL(mockCommentRepository, remove(_)).Times(0);
    EXPECT_THROW(commentService.remove(1, 20, 10),std::runtime_error);
}