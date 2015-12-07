/* 
 * File:   Calendar.cpp
 * Author: Patrik Čada
 * 
 * Created on 6. prosince 2015, 19:12
 */

#include "Calendar.h"
#include "CalendarEvent.h"
#include "Transition.h"
#include "Types.h"
#include <random>
#include <algorithm>
#include <map>
#include <iostream>

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
    
    //Smazání událostí
    for( TEventMap::iterator i = this->eventMap.begin(); i != this->eventMap.end(); i++ ){
        delete i->first;
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
    
    this->eventMap.insert( std::make_pair(event, time) );
    this->calendar.push_back(calendarItem);
    
    //Seřadíme kalendář.
    std::sort( this->calendar.begin(), this->calendar.end(), [](TCalendarItem *item1, TCalendarItem *item2){
        return (item1->first) < (item2->first);
    });
}
        
/**
 * Spuštění nejbližší události.
 * @return TPlaceVector *|null
 */
TPlaceVector *Calendar::runEvent( TTime maxTime ){
    TPlaceVector *changedPlaces = NULL;
    
    if( this->calendar.size() > 0 ){
        TCalendarItem *item = *(this->calendar.begin());
         
        if( item->first <= maxTime ){
            this->currentTime = item->first;
            
            //Spuětění události.
            TEventVector * events = item->second;
            CalendarEvent *event = *(events->begin());
            changedPlaces = event->run();
            
            //Smazání události.
            this->eventMap.erase(event);
            delete event;
            events->erase(events->begin());
            
            //Smazání položky z kalendáře.
            if( events->size() == 0 ){
                delete events;
                this->calendar.erase( this->calendar.begin() );
            }
        }
        
    }
    
    return changedPlaces;
}

/**
 * Odebrání události z kalendáře.
 * @param event
 */
void Calendar::removeEvent( CalendarEvent *event ){
    TTime time = this->eventMap[event];
    this->eventMap.erase(event);
    
    TCalendar::iterator i = std::find_if( this->calendar.begin(), this->calendar.end(), [time](TCalendarItem const *item){
        return item->first == time;
    });
    
    TEventVector *events = (*i)->second;
    TEventVector::iterator ii = std::find( events->begin(), events->end(), event );
    delete *ii;
    events->erase(ii);
    
    if( events->size() == 0 ){
        delete events;
        this->calendar.erase(i);
    }
}
        
/**
 * Aktuální čas v kalendáři
 * @return 
 */
const TTime * const Calendar::getCurrentTime(){
    return &this->currentTime;
}

/**
 * Genenerátor exponenciálních čísel
 * @param exp
 * @return 
 */
double Calendar::getExp( double exp ){
    return -exp * log( Calendar::getNorm() );
}
        
/**
 * Generátor náhodných čísel. 0.0 - 1.0
 * @return 
 */
double Calendar::getNorm(){
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(mt);
}

/**
 * Calc time for transition.
 * @param Transition transition
 * @return double
 */
double Calendar::calcTime( Transition *transition ){
    if( transition->getTimeType() == TTimeTypes::GeneratedTime)
        return Calendar::getExp( transition->getTime() );
    
    return transition->getTime();
}