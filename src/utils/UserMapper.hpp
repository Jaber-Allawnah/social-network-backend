#pragma once

#include "../models/User.hpp"
#include <mysqlx/xdevapi.h>

User mapRowToUser(const mysqlx::Row& row);