#include "./DateTimeUtils.hpp"

#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <spdlog/spdlog.h>

namespace DateTimeUtils {
    std::chrono::system_clock::time_point parseDateTime(const std::string& dateTime) {
        spdlog::debug("Parsing date-time value");

        std::tm time{};

        std::istringstream stream(dateTime);
        stream >> std::get_time(&time, "%Y-%m-%d %H:%M:%S");

        if (stream.fail()) {
            spdlog::warn("Failed to parse date-time value");
            throw std::runtime_error("DateTimeUtils: Failed to parse date-time");
        }

        std::time_t timeValue = std::mktime(&time);

        spdlog::debug("Date-time parsed successfully");
        return std::chrono::system_clock::from_time_t(timeValue);
    }

    std::string formatDateTime(const std::chrono::system_clock::time_point& timePoint) {
        spdlog::debug("Formatting date-time value");

        std::time_t timeValue = std::chrono::system_clock::to_time_t(timePoint);
        std::tm time{};

#ifdef _WIN32
        localtime_s(&time, &timeValue);
#else
        localtime_r(&timeValue, &time);
#endif

        std::ostringstream stream;
        stream << std::put_time(&time, "%Y-%m-%d %H:%M:%S");

        spdlog::debug("Date-time parsed successfully");
        return stream.str();
    }
}