/* 
 * File:   Transaction.cpp
 * Author: Patrik Čada
 * 
 * Created on 6. prosince 2015, 19:56
 */

#include "Transaction.h"

/**
 * Vytvoření transakce.
 * @param calendarTime
 * @param start
 */
Transaction::Transaction(const TTime * const calendarTime, Identificator *start): calendarTime(calendarTime){
    this->setCurrentPlace(start);
}

Transaction::~Transaction() {
}

/**
 * Nastavení aktuálního místa kde je transakce.
 * @param place
 */
void Transaction::setCurrentPlace( Identificator *place ){
    TTimeIdentificatorPair pair;
    
    pair.first  = *(this->calendarTime);
    pair.second = place;
    
    this->statistics.push_back( pair );
}
        
/**
 * Vrací statistiky o transakci.
 * @return 
 */
const TTimeIdentificators &Transaction::getStatistics(){
    return this->statistics;
}

/**
 * Je transakce živá?
 * @return 
 */
bool Transaction::isLive(){
    return this->live;
}