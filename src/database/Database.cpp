#include "Database.hpp"
#include <stdexcept>
#include <spdlog/spdlog.h>

Database::Database(const std::string& host,
	unsigned port,
	const std::string& username,
	const std::string& password,
	const std::string& databaseName) {
	spdlog::debug("Attempting database connection to {}:{} for database {}", host, port, databaseName);

	try {
		session_ = std::make_unique<mysqlx::Session>(mysqlx::SessionSettings(host,
												  	 port,
													 username,
													 password,
													 databaseName));

		spdlog::info("Database connection established successfully to {}:{} for database {}", host, port, databaseName);
	}
	catch (const mysqlx::Error& error) {
		spdlog::error("Database connection failed to {}:{} for database {}: {}", host, port, databaseName, error.what());
		throw std::runtime_error(std::string("Database connection failed: ") +
								 error.what());
	}
}

mysqlx::Session& Database::getSession() {
	spdlog::debug("Database session requested");
	return *session_;
}