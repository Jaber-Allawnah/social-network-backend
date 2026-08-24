#include "UserService.hpp"
#include <stdexcept>
#include "../utils/PasswordHasher.hpp"

UserService::UserService(UserRepository& userRepository) : userRepository_(userRepository) { }

User UserService::registerUser(const std::string& username,
                               const std::string& email,
                               const std::string& password) {
    if (userRepository_.getByEmail(email)) {
        throw std::runtime_error("User Service: Email is already in use");
    }

    if (userRepository_.getByUsername(username)) {
        throw std::runtime_error("User Service: Username is already in use");
    }

    std::string hashedPassword = hashPassword(password);
    return userRepository_.create(username, email, hashedPassword);
}

User UserService::login(const std::string& email,
                        const std::string& password) {
    auto user = userRepository_.getByEmail(email);
    if (!user) {
        throw std::runtime_error("User Service: User not registered");
    }

    if (!verifyPassword(password, user.value().passwordHash)) {
        throw std::runtime_error("User Service: Wrong password");
    }

    return user.value();
}