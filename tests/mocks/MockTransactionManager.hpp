#pragma once

#include <gmock/gmock.h>
#include "../../src/database/ITransactionManager.hpp"

class MockTransactionManager : public ITransactionManager {
public:
    MOCK_METHOD(void, begin, (), (override));
    MOCK_METHOD(void, commit, (), (override));
    MOCK_METHOD(void, rollback, (), (override));
};