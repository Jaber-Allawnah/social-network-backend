#include "./DateTimeUtils.hpp"

std::chrono::system_clock::time_point parseDateTime(const std::string& dateTime) {
    std::tm time{};

    std::istringstream stream(dateTime);
    stream >> std::get_time(&time, "%Y-%m-%d %H:%M:%S");

    std::time_t timeValue = std::mktime(&time);

    return std::chrono::system_clock::from_time_t(timeValue);
}
