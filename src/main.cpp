#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib> 
#include "database/Database.hpp"

std::string getEnvironmentVariable(const char* name) {
    const char* value = std::getenv(name);

    if (value == nullptr) {
        throw std::runtime_error(std::string("Missing environment variable: ") + name);
    }

    return std::string(value);
}

int main() {
    try {
        const std::string host = getEnvironmentVariable("DB_HOST");
        const unsigned port = static_cast<unsigned>(std::stoi(getEnvironmentVariable("DB_PORT")));
        const std::string username = getEnvironmentVariable("DB_USER");
        const std::string password = getEnvironmentVariable("DB_PASSWORD");
        const std::string databaseName = getEnvironmentVariable("DB_NAME");

        Database database(host,
                          port,
                          username,
                          password,
                          databaseName);
    }
    catch (const std::exception& error) {
        std::cerr << error.what() << std::endl;
        return 1;
    }

    return 0;
}