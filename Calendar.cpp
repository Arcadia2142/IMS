/* 
 * File:   Calendar.cpp
 * Author: Patrik Čada
 * 
 * Created on 6. prosince 2015, 19:12
 */

#include "Calendar.h"
#include "CalendarEvent.h"
#include <algorithm>

Calendar::Calendar() {
    
}


Calendar::~Calendar() {
    //Vyčištění kalendáře.
    for( TCalendar::iterator i = this->calendar.begin(); i != this->calendar.end(); i++){
        TCalendarItem *item = *i;
        delete item->second;
        delete item;
        
        this->calendar.erase(i);
    }
}

/**
 * Přidání události do kalendáře.
 * @param event
 * @param timeOffset
 */
void Calendar::addEvent( CalendarEvent *event, TTime timeOffset ){
    TTime time = this->currentTime + timeOffset;
    
    //Nalezení času v kalendáři.
    for( TCalendar::iterator i = this->calendar.begin(); i != this->calendar.end(); i++){
        TCalendarItem *item = *i;
        if( item->first == time ){
            item->second->push_back(event);
            return;
        }
    }
    
    //Čas neexistuje takže ho vytvoříme.
    TEventVector *eventVector = new TEventVector;
    eventVector->push_back(event);
    
    TCalendarItem *calendarItem = new TCalendarItem;
    calendarItem->first = time;
    calendarItem->second = eventVector;
    
    this->calendar.push_back(calendarItem);
    
    //Seřadíme kalendář.
    std::stable_sort( this->calendar.begin(), this->calendar.end() );
}
        
/**
 * Spuštění nejbližší události.
 * @return 
 */
bool Calendar::runEvent( TTime maxTime ){
    if( this->calendar.size() > 0 ){
        TCalendarItem *item = *(this->calendar.begin());
        if( item->first <= maxTime ){
            //Spuětění události.
            TEventVector * events = item->second;
            CalendarEvent *event = *(events->begin());
            event->run();
            
            //Smazání události.
            delete event;
            events->erase(events->begin());
            
            //Smazání položky z kalendáře.
            if( events->size() == 0 ){
                delete events;
                this->calendar.erase( this->calendar.begin() );
            }
            
            return true;
        }
    }
    
    return false;
}
        
/**
 * Aktuální čas v kalendáři
 * @return 
 */
const TTime * const Calendar::getCurrentTime(){
    return &this->currentTime;
}