#include "PasswordHasher.hpp"
#include <sodium.h>
#include <stdexcept>
#include <spdlog/spdlog.h>

namespace PasswordUtils {
    std::string hashPassword(const std::string& password) {
        spdlog::debug("Hashing password");

        char hashedPassword[crypto_pwhash_STRBYTES];
        if (crypto_pwhash_str(hashedPassword,
            password.c_str(),
            password.size(),
            crypto_pwhash_OPSLIMIT_INTERACTIVE,
            crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0) {

            spdlog::error("Failed to hash password");
            throw std::runtime_error("Password Hasher: Failed to hash password");
        }

        spdlog::debug("Password hashed successfully");

        return std::string(hashedPassword);
    }

    bool verifyPassword(const std::string& password,
        const std::string& passwordHash) {
        spdlog::debug("Verifying password");

        if (crypto_pwhash_str_verify(
            passwordHash.c_str(),
            password.c_str(),
            password.size()) != 0) {

            spdlog::debug("Password verification failed");
            return false;
        }

        spdlog::debug("Password verified successfully");

        return true;
    }
}