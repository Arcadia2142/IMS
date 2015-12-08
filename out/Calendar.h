/* 
 * File:   Calendar.h
 * Author: Patrik Čada
 *
 * Created on 6. prosince 2015, 19:12
 */

#ifndef CALENDAR_H
#define CALENDAR_H

#include <map>
#include "Types.h"
#include "Transition.h"
#include "CalendarEvent.h"

//Datový typy kalendáře.
typedef std::vector<CalendarEvent *> TEventVector;
typedef std::pair<TTime, TEventVector *> TCalendarItem; 
typedef std::vector<TCalendarItem *> TCalendar;
typedef std::map<CalendarEvent *, TTime> TEventMap;

/**
 * Kalendář plánování událostí.
 */
class Calendar {
    public:
        Calendar();
        virtual ~Calendar();
        
        /** Přidání události do kalendáře. */
        void addEvent( CalendarEvent *event, TTime timeOffset = 0 );
        
        /** Zrušení události. */
        void removeEvent( CalendarEvent *event );
        
        /** Spuštění nejbližší události. */
        TPlaceVector *runEvent( TTime maxTime );
        
        /** Aktuální čas v kalendáři */
        const TTime * const getCurrentTime();
        
        /** Genenerátor exponenciálních čísel */
        static double getExp( double exp );
        
        /** Generátor náhodných čísel. 0.0 - 1.0 */
        static double getNorm();
        
        /** Vypočítání času pro zadaný třechod.*/
        static double calcTime( Transition *transition );
    private:
        TTime currentTime = 0.0;
        
        TCalendar calendar;
        TEventMap eventMap;
};

#endif /* CALENDAR_H */

