/* 
 * File:   CalendarEvent.h
 * Author: Patrik Čada
 *
 * Created on 6. prosince 2015, 22:35
 */

#ifndef CALENDAREVENT_H
#define CALENDAREVENT_H

#include "Types.h"

/**
 * Událost v kalendáři.
 * (Abstraktní třída)
 */
class CalendarEvent {
    public:
        CalendarEvent( Calendar *calendar, TransactionProvider *transactionProvider );

        virtual ~CalendarEvent();
        
        /** Spuštění události. */
        virtual void run() = 0;
    protected:
        /** Objekt kalendáře. */
        Calendar * const calendar;
        
        /** Provider na vytváření transakcí. */
        TransactionProvider *const transactionProvider;
};

#endif /* CALENDAREVENT_H */

