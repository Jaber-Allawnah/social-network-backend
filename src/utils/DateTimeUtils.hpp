#pragma once

#include <string>
#include <chrono>

namespace DateTimeUtils {
	std::chrono::system_clock::time_point parseDateTime(const std::string& dateTime);

	std::string formatDateTime(const std::chrono::system_clock::time_point& timePoint);
}


