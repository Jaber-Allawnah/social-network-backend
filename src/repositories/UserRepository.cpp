#include "UserRepository.hpp"
#include "../utils/DateTimeUtils.hpp"
#include <mysqlx/xdevapi.h>
#include <ctime>
#include <iomanip>
#include <sstream>

UserRepository::UserRepository(Database& database) : database_(database) {}

User UserRepository::mapRowToUser(const mysqlx::Row& row) const {
    User user;

    user.id = static_cast<int>(row[0]);
    user.username = static_cast<std::string>(row[1]);
    user.email = static_cast<std::string>(row[2]);
    user.passwordHash = static_cast<std::string>(row[3]);
    user.createdAt = parseDateTime(static_cast<std::string>(row[4]));
    user.updatedAt = parseDateTime(static_cast<std::string>(row[5]));

    return user;
}

std::optional<User> UserRepository::getById(int id)
{
    mysqlx::Session& session = database_.getSession();

    mysqlx::SqlResult result = session.sql("SELECT id, username, email, password_hash, "
                                           "DATE_FORMAT(created_at, '%Y-%m-%d %H:%i:%s'), "
                                           "DATE_FORMAT(updated_at, '%Y-%m-%d %H:%i:%s') "
                                           "FROM users WHERE id = ?").bind(id).execute();

    mysqlx::Row row = result.fetchOne();

    if (!row)
    {
        return std::nullopt;
    }

    User user = mapRowToUser(row);

    return user;
}

std::vector<User> UserRepository::getAllUsers()
{
    mysqlx::Session& session = database_.getSession();

    mysqlx::SqlResult result = session.sql("SELECT id, username, email, password_hash, "
                                           "DATE_FORMAT(created_at, '%Y-%m-%d %H:%i:%s'), "
                                           "DATE_FORMAT(updated_at, '%Y-%m-%d %H:%i:%s') "
                                           "FROM users").execute();

    auto rows = result.fetchAll();

    std::vector<User> allUsers;

    for (const mysqlx::Row& row : rows)
    {
        allUsers.push_back(mapRowToUser(row));
    }

    return allUsers;
}

void UserRepository::addUser(const std::string& username,
                             const std::string& email,
                             const std::string& passwordHash)
{
    mysqlx::Session& session = database_.getSession();

    session.sql("INSERT INTO users (username, email, password_hash) "
                "VALUES (?, ?, ?)").bind(username, email, passwordHash).execute();
}

void UserRepository::updateUser(int id,
                                const std::string& username,
                                const std::string& email,
                                const std::string& passwordHash)
{
    mysqlx::Session& session = database_.getSession();

    session.sql("UPDATE users "
                "SET username = ?, email = ?, password_hash = ? "
                "WHERE id = ?").bind(username, email, passwordHash, id).execute();
}

void UserRepository::deleteUser(int id) {
    mysqlx::Session& session = database_.getSession();

    session.sql("DELETE FROM users "
                "WHERE id = ?").bind(id).execute();
}

