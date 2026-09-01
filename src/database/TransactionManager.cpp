#include "TransactionManager.hpp"

TransactionManager::TransactionManager(Database& database)
                                     : database_(database) {}

void TransactionManager::begin() {
    database_.getSession().startTransaction();
}

void TransactionManager::commit() {
    database_.getSession().commit();
}

void TransactionManager::rollback() {
    database_.getSession().rollback();
}