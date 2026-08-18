#pragma once

#include <chrono>
struct Follow {
	int followerId;
	int followeeId;
	std::chrono::system_clock::time_point createdAt;
};