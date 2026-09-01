#pragma once

#include "../models/User.hpp"
#include <optional>
#include <string>

class IUserRepository {
public:
    virtual ~IUserRepository() = default;

    virtual std::optional<User> getById(int userId) = 0;
    virtual std::optional<User> getByUsername(const std::string& username) = 0;
    virtual std::optional<User> getByEmail(const std::string& email) = 0;

    virtual User create(const std::string& username,
                        const std::string& email,
                        const std::string& passwordHash) = 0;
    virtual bool update(int userId,
                        const std::string& username,
                        const std::string& email,
                        const std::string& passwordHash) = 0;
    virtual bool remove(int userId) = 0;
};