#pragma once

#include <gmock/gmock.h>
#include "../../src/repositories/IUserRepository.hpp"

class MockUserRepository : public IUserRepository {
public:
    MOCK_METHOD(std::optional<User>, getById, (int userId), (override));
    MOCK_METHOD(std::optional<User>, getByUsername, (const std::string& username), (override));
    MOCK_METHOD(std::optional<User>, getByEmail, (const std::string& email), (override));

    MOCK_METHOD(User, create,
                (const std::string& username,
                 const std::string& email,
                 const std::string& passwordHash),
                (override));

    MOCK_METHOD(bool, update, 
                (int userId,
                 const std::string& username,
                 const std::string& email,
                 const std::string& passwordHash),
                (override));

    MOCK_METHOD(bool, remove, (int userId), (override));
};