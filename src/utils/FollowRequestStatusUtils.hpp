#pragma once

#include "../models/FollowRequestStatus.hpp"
#include <string>

FollowRequestStatus parseFollowRequestStatus(const std::string& status);

std::string followRequestStatusToString(FollowRequestStatus status);