#pragma once

#include <string>
#include <chrono>

struct User
{
	int id;
	std::string username;
	std::string email;
	std::string passwordHash;
	std::chrono::system_clock::time_point createdAt;
	std::chrono::system_clock::time_point updatedAt;
};