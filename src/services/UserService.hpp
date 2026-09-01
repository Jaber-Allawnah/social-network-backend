#pragma once

#include "../repositories/IUserRepository.hpp"

class UserService {
private:
    IUserRepository& userRepository_;
public:
    UserService(IUserRepository& userRepository);

	User registerUser(const std::string& username,
                      const std::string& email,
                      const std::string& password);
    User login(const std::string& email,
               const std::string& password);
};