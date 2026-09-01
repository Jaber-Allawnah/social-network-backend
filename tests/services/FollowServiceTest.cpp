#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../../src/services/FollowService.hpp"

#include "../mocks/MockTransactionManager.hpp"
#include "../mocks/MockFollowRepository.hpp"
#include "../mocks/MockUserRepository.hpp"
#include "../mocks/MockFollowRequestRepository.hpp"

using testing::_;
using testing::Return;

class FollowServiceTest : public ::testing::Test {
protected:
	MockFollowRepository mockFollowRepository;
	MockFollowRequestRepository mockFollowRequestRepository;
	MockUserRepository mockUserRepository;
	MockTransactionManager mockTransactionManager;

	FollowService followService{mockFollowRepository, 
								mockFollowRequestRepository, 
								mockUserRepository, 
								mockTransactionManager };

	User requester{1, "Jaber", "jaber@example.com", "hash1"};
	User receiver{2, "Raghad", "raghad@example.com", "hash2"};
};

TEST_F(FollowServiceTest, SendFollowRequestSuccessfully) {
	EXPECT_CALL(mockUserRepository, getById(1)).WillOnce(Return(requester));
	EXPECT_CALL(mockUserRepository, getById(2)).WillOnce(Return(receiver));
	EXPECT_CALL(mockFollowRepository, isFollowing(1, 2)).WillOnce(Return(false));
	EXPECT_CALL(mockFollowRequestRepository, getByUsers(1, 2)).WillOnce(Return(std::nullopt));
	EXPECT_CALL(mockFollowRequestRepository, create(1, 2)).WillOnce(Return(true));

	bool result = followService.sendFollowRequest(1, 2);

	EXPECT_TRUE(result);
}

TEST_F(FollowServiceTest, SendFollowRequestThrowsWhenFollowingSelf) {
	EXPECT_THROW(followService.sendFollowRequest(1, 1),std::runtime_error);
}

TEST_F(FollowServiceTest, SendFollowRequestThrowsWhenRequestAlreadyExists) {
	FollowRequest followRequest{1, 1, 2, FollowRequestStatus::Pending};

	EXPECT_CALL(mockUserRepository, getById(1)).WillOnce(Return(requester));
	EXPECT_CALL(mockUserRepository, getById(2)).WillOnce(Return(receiver));
	EXPECT_CALL(mockFollowRequestRepository, getByUsers(1, 2)).WillOnce(Return(followRequest));
	EXPECT_CALL(mockFollowRepository, isFollowing(1, 2)).WillOnce(Return(false));

	EXPECT_THROW(followService.sendFollowRequest(1, 2), std::runtime_error);
}

TEST_F(FollowServiceTest, AcceptFollowRequestSuccessfully) {
	FollowRequest followRequest{1, 1, 2, FollowRequestStatus::Pending};

	EXPECT_CALL(mockUserRepository, getById(2)).WillOnce(Return(receiver));
	EXPECT_CALL(mockFollowRequestRepository, getById(1)).WillOnce(Return(followRequest));
	EXPECT_CALL(mockFollowRequestRepository, update(1, FollowRequestStatus::Accepted)).WillOnce(Return(true));
	EXPECT_CALL(mockFollowRepository, follow(followRequest.requesterId, 2)).WillOnce(Return(true));
	EXPECT_CALL(mockTransactionManager, begin()).Times(1);
	EXPECT_CALL(mockTransactionManager, commit()).Times(1);
	EXPECT_CALL(mockTransactionManager, rollback()).Times(0);

	bool result = followService.acceptFollowRequest(1, 2);
	EXPECT_TRUE(result);	
}

TEST_F(FollowServiceTest, RejectFollowRequestSuccessfully) {
	FollowRequest followRequest{1, 1, 2, FollowRequestStatus::Pending};

	EXPECT_CALL(mockUserRepository, getById(2)).WillOnce(Return(receiver));
	EXPECT_CALL(mockFollowRequestRepository, getById(1)).WillOnce(Return(followRequest));
	EXPECT_CALL(mockFollowRequestRepository, update(1, FollowRequestStatus::Rejected)).WillOnce(Return(true));

	bool result = followService.rejectFollowRequest(1, 2);
	EXPECT_TRUE(result);
}