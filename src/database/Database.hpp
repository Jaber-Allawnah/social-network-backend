#pragma once

#include <string>
#include <mysqlx/xdevapi.h>

class Database {
private:
	mysqlx::Session session_;

public:
	Database(
		const std::string& host,
		int port,
		const std::string& username,
		const std::string& password,
		const std::string& databaseName
	);

	mysqlx::Session& getSession();
};
