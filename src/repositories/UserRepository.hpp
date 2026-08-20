#pragma once

#include "../database/Database.hpp"
#include "../models/User.hpp"
#include <mysqlx/xdevapi.h>
#include <string>
#include <optional>
#include <vector>


class UserRepository {
private:
	Database& database_;

public:
	UserRepository(Database& database);

	std::optional<User> getById(int userId);
	std::optional<User> getByUsername(const std::string& username);
	std::optional<User> getByEmail(const std::string& email);

	void create(const std::string& username,
				const std::string& email,
			 const std::string& passwordHash);
	bool update(int userId,
				const std::string& username,
				const std::string& email,
				const std::string& passwordHash);
	bool remove(int userId);
};	