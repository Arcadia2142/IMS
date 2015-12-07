/* 
 * File:   CalendarEvent.cpp
 * Author: Patrik Čada
 * 
 * Created on 6. prosince 2015, 22:35
 */

#include "CalendarEvent.h"

/**
 * Vytvoření události v kalendáři.
 * @param calendar
 */
CalendarEvent::CalendarEvent( Calendar *calendar, TransactionProvider *transactionProvider ): 
    calendar(calendar),
    transactionProvider(transactionProvider)
{
    
}

CalendarEvent::~CalendarEvent() {
}

