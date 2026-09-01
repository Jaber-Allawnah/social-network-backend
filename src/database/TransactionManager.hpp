#pragma once

#include "ITransactionManager.hpp"
#include "Database.hpp"

class TransactionManager : public ITransactionManager {
private:
    Database& database_;

public:
    explicit TransactionManager(Database& database);

    void begin() override;
    void commit() override;
    void rollback() override;
};