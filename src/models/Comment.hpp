#pragma once

#include <string>
#include <chrono>

struct Comment {
    int id;
    std::string content;
    int userId;
    int postId;
    std::chrono::system_clock::time_point createdAt;
    std::chrono::system_clock::time_point updatedAt;
};