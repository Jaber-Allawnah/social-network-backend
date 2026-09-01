#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../../src/services/PostService.hpp"

#include "../mocks/MockPostRepository.hpp"
#include "../mocks/MockFollowRepository.hpp"
#include "../mocks/MockUserRepository.hpp"

using ::testing::Return;
using ::testing::_;

class PostServiceTest : public ::testing::Test {
protected:	
    MockPostRepository mockPostRepository;
    MockFollowRepository mockFollowRepository;
    MockUserRepository mockUserRepository;

    PostService postService{mockPostRepository, mockFollowRepository, mockUserRepository};

    User user{ 1, "Jaber", "jaber@example.com", "hashed_password" };
    Post post{ 10, "Old content", 1 };
};

TEST_F(PostServiceTest, CreatePostSuccessfully) {
    EXPECT_CALL(mockUserRepository, getById(1)).WillOnce(Return(user));
    EXPECT_CALL(mockPostRepository, create(1, "Hello")).WillOnce(Return(Post{10, "Hello", 1}));

    Post result = postService.create(1, "Hello");

    EXPECT_EQ(result.id, 10);
    EXPECT_EQ(result.userId, 1);
    EXPECT_EQ(result.content, "Hello");
}

TEST_F(PostServiceTest, UpdatePostSuccessfully) {
    EXPECT_CALL(mockUserRepository, getById(1)).WillOnce(Return(user));
    EXPECT_CALL(mockPostRepository, getById(10)).WillOnce(Return(post));
    EXPECT_CALL(mockPostRepository, update(10, "New Content")).WillOnce(Return(true));

    bool result = postService.update(10, 1, "New Content");
    EXPECT_TRUE(result);
}

TEST_F(PostServiceTest, RemovePostSuccessfully) {
    EXPECT_CALL(mockUserRepository, getById(1)).WillOnce(Return(user));
    EXPECT_CALL(mockPostRepository, getById(10)).WillOnce(Return(post));
    EXPECT_CALL(mockPostRepository, remove(10)).WillOnce(Return(true));

    bool result = postService.remove(10, 1);
    EXPECT_TRUE(result);
}

TEST_F(PostServiceTest, UpdateThrowsWhenPostDoesNotExist) {
    EXPECT_CALL(mockUserRepository, getById(1)).WillOnce(Return(user));
    EXPECT_CALL(mockPostRepository, getById(10)).WillOnce(Return(std::nullopt));
    EXPECT_CALL(mockPostRepository, update(_, _)).Times(0);

    EXPECT_THROW(postService.update(10, 1, "New Content"), std::runtime_error);
}

TEST_F(PostServiceTest, UpdateThrowsWhenUserDoesNotOwnPost) {
    User anotherUser{2, "Raghad", "raghad@example.com", "hashed_password" };

    EXPECT_CALL(mockUserRepository, getById(2)).WillOnce(Return(anotherUser));
    EXPECT_CALL(mockPostRepository, getById(10)).WillOnce(Return(post));
    EXPECT_CALL(mockPostRepository, update(_, _)).Times(0);

    EXPECT_THROW(postService.update(10, 2, "New Content"), std::runtime_error);
}

TEST_F(PostServiceTest, RemoveThrowsWhenUserDoesNotOwnPost) {
    User anotherUser{ 2, "Raghad", "raghad@example.com", "hashed_password" };

    EXPECT_CALL(mockUserRepository, getById(2)).WillOnce(Return(anotherUser));
    EXPECT_CALL(mockPostRepository, getById(10)).WillOnce(Return(post));
    EXPECT_CALL(mockPostRepository, remove(_)).Times(0);
    
    EXPECT_THROW(postService.remove(10, 2), std::runtime_error);
}
