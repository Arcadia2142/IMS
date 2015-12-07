/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Transaction.h
 * Author: Patrik Čada
 *
 * Created on 6. prosince 2015, 19:56
 */

#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "Types.h"
#include "Identificator.h"

/**
 * Transakce v petriho síti.
 * @param calendarTime
 * @param start
 */
class Transaction {
    public:
        Transaction( const TTime * const calendarTime, Identificator *start );
        virtual ~Transaction();
        
        /** Nastavení aktuálního místa kde se transakce nachází (místo/hrana/přechod) */
        void setCurrentPlace( Identificator *place );
        
        
        
        /** Je transakce naživu? */
        bool isLive();
        
        /** Vrací statistiky o transakci */
        const TTimeIdentificators &getStatistics();
    private:
        /** Aktuální modelový čas. */
        const TTime * const calendarTime; 
        
        /** Statistiky transakce. */
        TTimeIdentificators statistics;
        
        /** Stav života transakce. */
        bool live = true;
        
        /** Počítadlo  */
        static unsigned int identificator;
};

#endif /* TRANSACTION_H */

