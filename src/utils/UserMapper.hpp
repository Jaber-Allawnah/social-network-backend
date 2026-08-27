#pragma once

#include "../models/User.hpp"
#include <mysqlx/xdevapi.h>

namespace UserMapper {

	User mapRowToUser(const mysqlx::Row& row);

}