#include "./DateTimeUtils.hpp"

#include <ctime>
#include <iomanip>
#include <sstream>

namespace DateTimeUtils {
    std::chrono::system_clock::time_point parseDateTime(const std::string& dateTime) {
        std::tm time{};

        std::istringstream stream(dateTime);
        stream >> std::get_time(&time, "%Y-%m-%d %H:%M:%S");

        std::time_t timeValue = std::mktime(&time);

        return std::chrono::system_clock::from_time_t(timeValue);
    }

    std::string formatDateTime(const std::chrono::system_clock::time_point& timePoint) {
        std::time_t timeValue = std::chrono::system_clock::to_time_t(timePoint);
        std::tm time{};

        #ifdef _WIN32
                localtime_s(&time, &timeValue);
        #else
                localtime_r(&timeValue, &time);
        #endif

        std::ostringstream stream;
        stream << std::put_time(&time, "%Y-%m-%d %H:%M:%S");

        return stream.str();
    }
}