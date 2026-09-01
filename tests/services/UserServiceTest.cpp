#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../../src/services/UserService.hpp"
#include "../mocks/MockUserRepository.hpp"
#include "../../src/utils/PasswordHasher.hpp"

using namespace PasswordUtils;
using ::testing::Return;
using ::testing::_;

class UserServiceTest : public ::testing::Test {
protected:
	MockUserRepository mockUserRepository;

	UserService userService{ mockUserRepository };

	User user{ 1, "Jaber", "jaber@example.com", "hashed_password" };
};

TEST_F(UserServiceTest, TestSuccessfulRegistration) {
	EXPECT_CALL(mockUserRepository, getByEmail("jaber@example.com")).WillOnce(Return(std::nullopt));
	EXPECT_CALL(mockUserRepository, getByUsername("Jaber")).WillOnce(Return(std::nullopt));
	EXPECT_CALL(mockUserRepository, create("Jaber", "jaber@example.com", _)).WillOnce(Return(user));

	User result = userService.registerUser("Jaber", "jaber@example.com", "1234567");
	EXPECT_EQ(result.id, user.id);
	EXPECT_EQ(result.username, user.username);
	EXPECT_EQ(result.email, user.email);
	EXPECT_EQ(result.passwordHash, user.passwordHash);
}

TEST_F(UserServiceTest, TestFailedRegistrationExistingUsername) {
	EXPECT_CALL(mockUserRepository, getByEmail("jaber@example.com")).WillOnce(Return(std::nullopt));
	EXPECT_CALL(mockUserRepository, getByUsername("Jaber")).WillOnce(Return(user));
	EXPECT_CALL(mockUserRepository, create(_, _, _)).Times(0);

	EXPECT_THROW(userService.registerUser("Jaber", "jaber@example.com", "1234567"), std::runtime_error);
}

TEST_F(UserServiceTest, TestFailedRegistrationExistingEmail) {
	EXPECT_CALL(mockUserRepository, getByEmail("jaber@example.com")).WillOnce(Return(user));
	EXPECT_CALL(mockUserRepository, create(_, _, _)).Times(0);

	EXPECT_THROW(userService.registerUser("Jaber", "jaber@example.com", "1234567"), std::runtime_error);
}

TEST_F(UserServiceTest, TestSuccessfulLogin) {
	std::string password = "1234567";
	std::string hashedPassword = hashPassword(password);
	User user{ 1, "Jaber", "jaber@example.com", hashedPassword };

	EXPECT_CALL(mockUserRepository, getByEmail("jaber@example.com")).WillOnce(Return(user));

	User result = userService.login("jaber@example.com", password);
	EXPECT_EQ(result.id, user.id);
	EXPECT_EQ(result.username, user.username);
	EXPECT_EQ(result.email, user.email);
	EXPECT_EQ(result.passwordHash, user.passwordHash);
}

TEST_F(UserServiceTest, TestFailedLoginNonExistingUser) {
	EXPECT_CALL(mockUserRepository, getByEmail("jaber@example.com")).WillOnce(Return(std::nullopt));
	
	EXPECT_THROW(userService.login("jaber@example.com", "12345678"), std::runtime_error);
}

TEST_F(UserServiceTest, TestFailedLoginWrongPassword) {
	std::string password = "1234567";
	std::string hashedPassword = hashPassword(password);
	User user{ 1, "Jaber", "jaber@example.com", hashedPassword };

	EXPECT_CALL(mockUserRepository, getByEmail("jaber@example.com")).WillOnce(Return(user));
	EXPECT_THROW(userService.login("jaber@example.com", "12345678"), std::runtime_error);
}