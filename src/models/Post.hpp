#pragma once

#include <string>
#include <chrono>

struct Post {
	int id;
	std::string content;
	int userId;
	std::chrono::system_clock::time_point createdAt;
	std::chrono::system_clock::time_point updatedAt;
};