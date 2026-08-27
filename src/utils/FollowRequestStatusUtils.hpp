#pragma once

#include "../models/FollowRequestStatus.hpp"
#include <string>

namespace FollowRequestStatusUtils {
	FollowRequestStatus parseFollowRequestStatus(const std::string& status);

	std::string followRequestStatusToString(FollowRequestStatus status);
}