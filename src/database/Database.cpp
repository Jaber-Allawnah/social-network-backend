#include "Database.hpp"
#include <stdexcept>

Database::Database(const std::string& host,
				   unsigned port,
				   const std::string& username,
				   const std::string& password,
				   const std::string& databaseName) {
	try {
		session_ = std::make_unique<mysqlx::Session>(mysqlx::SessionSettings(host,
																			  port,
																			  username,
																			  password,
																			  databaseName));
	}
	catch (const mysqlx::Error& error) {
		throw std::runtime_error(std::string("Database connection failed: ") +
			error.what());
	}
}

mysqlx::Session& Database::getSession(){
	return *session_;
}
