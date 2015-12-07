/* 
 * File:   TTransactionProvider.cpp
 * Author: Patrik Čada
 * 
 * Created on 7. prosince 2015, 0:17
 */

#include "TransactionProvider.h"
#include "Transaction.h"

TransactionProvider::TransactionProvider( TTransactionVector *transactions, const TTime * const currentTime ):
transactions(transactions),
currentTime(currentTime)
{
    
}

TransactionProvider::~TransactionProvider() {
}

/**
 * Vytvoření nové transakce.
 * @param identificator
 * @return 
 */
Transaction *TransactionProvider::createTransaction( Identificator *identificator ){
    Transaction *transaction = new Transaction( this->currentTime, identificator );
    this->transactions->push_back(transaction);
    return transaction;
}