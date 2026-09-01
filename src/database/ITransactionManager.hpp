#pragma once

class ITransactionManager {
public:
    virtual ~ITransactionManager() = default;

    virtual void begin() = 0;
    virtual void commit() = 0;
    virtual void rollback() = 0;
};