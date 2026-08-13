#pragma once

#include <string>
#include <memory>
#include <mysqlx/xdevapi.h>

class Database {
private:
	std::unique_ptr<mysqlx::Session> session_;

public:
	Database(const std::string& host,
			 unsigned  port,
			 const std::string& username,
			 const std::string& password,
			 const std::string& databaseName);

	mysqlx::Session& getSession();
};
