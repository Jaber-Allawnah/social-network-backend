#include "UserRepository.hpp"
#include "../utils/DateTimeUtils.hpp"
#include "../utils/UserMapper.hpp"
#include <mysqlx/xdevapi.h>
#include <stdexcept>

using namespace UserMapper;

UserRepository::UserRepository(Database& database) : database_(database) {}

std::optional<User> UserRepository::getById(int userId) {
    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            SELECT
                id,
                username,
                email,
                password_hash,
                DATE_FORMAT(created_at, '%Y-%m-%d %H:%i:%s'),
                DATE_FORMAT(updated_at, '%Y-%m-%d %H:%i:%s')
            FROM users
            WHERE id = ?)";

        mysqlx::SqlResult result = session.sql(sql).bind(userId).execute();
        mysqlx::Row row = result.fetchOne();
        if (!row) {
            return std::nullopt;
        }

        User user = mapRowToUser(row);

        return user;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("UserRepository: Failed to retrieve user by user id: " + std::string(error.what()));
    }
}

std::optional<User> UserRepository::getByUsername(const std::string& username) {
    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            SELECT
                id,
                username,
                email,
                password_hash,
                DATE_FORMAT(created_at, '%Y-%m-%d %H:%i:%s'),
                DATE_FORMAT(updated_at, '%Y-%m-%d %H:%i:%s')
            FROM users
            WHERE username = ?)";

        mysqlx::SqlResult result = session.sql(sql).bind(username).execute();
        mysqlx::Row row = result.fetchOne();
        if (!row)
            return std::nullopt;

        User user = mapRowToUser(row);

        return user;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("UserRepository: Failed to retrieve user by username: " + std::string(error.what()));
    }
}

std::optional<User> UserRepository::getByEmail(const std::string& email) {
    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            SELECT
                id,
                username,
                email,
                password_hash,
                DATE_FORMAT(created_at, '%Y-%m-%d %H:%i:%s'),
                DATE_FORMAT(updated_at, '%Y-%m-%d %H:%i:%s')
            FROM users
            WHERE email = ?)";

        mysqlx::SqlResult result = session.sql(sql).bind(email).execute();
        mysqlx::Row row = result.fetchOne();
        if (!row)
            return std::nullopt;

        User user = mapRowToUser(row);

        return user;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("UserRepository: Failed to retrieve user by email: " + std::string(error.what()));
    }
}

User UserRepository::create(const std::string& username,
    const std::string& email,
    const std::string& passwordHash) {
    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            INSERT INTO users (username, email, password_hash)
            VALUES (?, ?, ?))";

        mysqlx::SqlResult result = session.sql(sql).bind(username, email, passwordHash).execute();

        int userId = static_cast<int>(result.getAutoIncrementValue());
        std::optional<User> user = getById(userId);
        if (!user) {
            throw std::runtime_error("UserRepository: Created user could not be retrieved");
        }

        return user.value();
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("UserRepository: Failed to create user: " + std::string(error.what()));
    }
}

bool UserRepository::update(int userId,
    const std::string& username,
    const std::string& email,
    const std::string& passwordHash) {
    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            UPDATE users
            SET username = ?, email = ?, password_hash = ?
            WHERE id = ?)";

        mysqlx::SqlResult result = session.sql(sql).bind(username, email, passwordHash, userId).execute();

        return result.getAffectedItemsCount() > 0;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("UserRepository: Failed to update user: " + std::string(error.what()));
    }
}

bool UserRepository::remove(int userId) {
    try {
        mysqlx::Session& session = database_.getSession();
        const std::string sql = R"(
            DELETE FROM users
            WHERE id = ?)";

        mysqlx::SqlResult result = session.sql(sql).bind(userId).execute();

        return result.getAffectedItemsCount() > 0;
    }
    catch (const mysqlx::Error& error) {
        throw std::runtime_error("UserRepository: Failed to remove user: " + std::string(error.what()));
    }
}