/* 
 * File:   Calendar.h
 * Author: Patrik Čada
 *
 * Created on 6. prosince 2015, 19:12
 */

#ifndef CALENDAR_H
#define CALENDAR_H

#include "Types.h"

//Datový typy kalendáře.
typedef std::vector<CalendarEvent *> TEventVector;
typedef std::pair<TTime, TEventVector *> TCalendarItem; 
typedef std::vector<TCalendarItem *> TCalendar;

/**
 * Kalendář plánování událostí.
 */
class Calendar {
    public:
        Calendar();
        virtual ~Calendar();
        
        /** Přidání události do kalendáře. */
        void addEvent( CalendarEvent *event, TTime timeOffset = 0 );
        
        /** Spuštění nejbližší události. */
        bool runEvent( TTime maxTime ); 
        
        /** Aktuální čas v kalendáři */
        const TTime * const getCurrentTime();
    private:
        TTime currentTime = 0;
        
        TCalendar calendar;
};

#endif /* CALENDAR_H */

