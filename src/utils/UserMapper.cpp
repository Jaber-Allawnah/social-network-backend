#include "./UserMapper.hpp"
#include "./DateTimeUtils.hpp"

using namespace DateTimeUtils;

namespace UserMapper {
    User mapRowToUser(const mysqlx::Row& row)
    {
        User user;
        user.id = static_cast<int>(row[0]);
        user.username = static_cast<std::string>(row[1]);
        user.email = static_cast<std::string>(row[2]);
        user.passwordHash = static_cast<std::string>(row[3]);
        user.createdAt = parseDateTime(static_cast<std::string>(row[4]));
        user.updatedAt = parseDateTime(static_cast<std::string>(row[5]));

        return user;
    }
}
