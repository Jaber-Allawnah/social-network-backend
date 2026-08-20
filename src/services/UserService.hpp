#pragma once

#include "../repositories/UserRepository.hpp"

class UserService {
private:
    UserRepository& userRepository_;
public:
    UserService(UserRepository& userRepository);

	void registerUser(const std::string& username,
                      const std::string& email,
                      const std::string& password);
    User login(const std::string& email,
               const std::string& password);
};