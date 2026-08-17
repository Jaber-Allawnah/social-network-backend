#pragma once

#include "../database/Database.hpp"
#include "../models/User.hpp"
#include <mysqlx/xdevapi.h>
#include <string>
#include <chrono>
#include <optional>
#include <vector>


class UserRepository {
private:
	Database& database_;

	User mapRowToUser(const mysqlx::Row&) const;

public:
	UserRepository(Database& database);

	// Get Operations
	std::optional<User> getById(int id);
	std::vector<User> getAllUsers();

	// Add Operation
	void addUser(const std::string& username,
				 const std::string& email,
				 const std::string& passwordHash);
	
	// Update Operation
	void updateUser(int id,
					const std::string& username,
					const std::string& email,
					const std::string& passwordHash);

	// Delete Operation 
	void deleteUser(int id);

};	