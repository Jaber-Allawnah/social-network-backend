#include "UserService.hpp"
#include <stdexcept>
#include "../utils/PasswordHasher.hpp"
#include <iostream>
#include <spdlog/spdlog.h>

using namespace PasswordUtils;

UserService::UserService(UserRepository& userRepository) : userRepository_(userRepository) {}

User UserService::registerUser(const std::string& username,
    const std::string& email,
    const std::string& password) {
    spdlog::debug("Registering user with username {}", username);

    if (username.empty() || email.empty() || password.empty()) {
        spdlog::warn("User registration rejected: one or more required inputs are empty");
        throw std::runtime_error("User Service: Inputs can't be empty");
    }

    if (userRepository_.getByEmail(email)) {
        spdlog::warn("User registration rejected: email is already in use");
        throw std::runtime_error("User Service: Email is already in use");
    }

    if (userRepository_.getByUsername(username)) {
        spdlog::warn("User registration rejected: username {} is already in use", username);
        throw std::runtime_error("User Service: Username is already in use");
    }

    std::string hashedPassword = hashPassword(password);
    User user = userRepository_.create(username, email, hashedPassword);

    spdlog::info("User {} registered successfully", user.id);

    return user;
}

User UserService::login(const std::string& email,
    const std::string& password) {
    spdlog::debug("Login attempt started");

    auto user = userRepository_.getByEmail(email);
    if (!user) {
        spdlog::warn("Login rejected: user not registered");
        throw std::runtime_error("User Service: User not registered");
    }

    if (!verifyPassword(password, user.value().passwordHash)) {
        spdlog::warn("Login rejected: invalid credentials for user {}", user.value().id);
        throw std::runtime_error("User Service: Wrong password");
    }

    spdlog::info("User {} logged in successfully", user.value().id);

    return user.value();
}