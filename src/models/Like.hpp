#pragma once

#include <chrono>

struct Like {
	int userId;
	int postId;
	std::chrono::system_clock::time_point createdAt;
};