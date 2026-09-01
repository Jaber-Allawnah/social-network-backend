#pragma once

#include "../database/Database.hpp"
#include "../models/User.hpp"
#include "./IUserRepository.hpp"
#include <mysqlx/xdevapi.h>
#include <string>
#include <optional>
#include <vector>


class UserRepository : public IUserRepository{
private:
	Database& database_;

public:
	UserRepository(Database& database);

	std::optional<User> getById(int userId) override;
	std::optional<User> getByUsername(const std::string& username) override;
	std::optional<User> getByEmail(const std::string& email) override;

	User create(const std::string& username,
				const std::string& email,
				const std::string& passwordHash) override;
	bool update(int userId,
				const std::string& username,
				const std::string& email,
				const std::string& passwordHash) override;
	bool remove(int userId) override;
};	