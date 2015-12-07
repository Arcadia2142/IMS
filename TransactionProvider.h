/* 
 * File:   TTransactionProvider.h
 * Author: Patrik Čada
 *
 * Created on 7. prosince 2015, 0:17
 */

#ifndef TTRANSACTIONPROVIDER_H
#define TTRANSACTIONPROVIDER_H

#include "Types.h"

/**
 * Třída umožňující vytvářet nové transakce.
 * @param transactions
 */
class TransactionProvider{
    public:
        TransactionProvider( TTransactionVector *transactions, const TTime * const currentTime );
        virtual ~TransactionProvider();
        
        Transaction *createTransaction( Identificator *identificator );
    private:
       /** Vektor transakcí. */
       TTransactionVector *transactions;
       
       /** Aktuální čas v kalendáři. */
       const TTime * const currentTime;
};

#endif /* TTRANSACTIONPROVIDER_H */

