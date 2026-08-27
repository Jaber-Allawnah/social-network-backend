#include "PasswordHasher.hpp"
#include <sodium.h>
#include <stdexcept>

std::string hashPassword(const std::string& password) {
    char hashedPassword[crypto_pwhash_STRBYTES];
    if (crypto_pwhash_str(hashedPassword,
                          password.c_str(),
                          password.size(),
                          crypto_pwhash_OPSLIMIT_INTERACTIVE,
                          crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0) {
        throw std::runtime_error("Password Hasher: Failed to hash password");
    }

    return std::string(hashedPassword);
}

bool verifyPassword(const std::string& password,
                    const std::string& passwordHash) {
    if (crypto_pwhash_str_verify(
        passwordHash.c_str(),
        password.c_str(),
        password.size()) != 0) {
        
        return false;
    }

    return true;
}