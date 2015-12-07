/* 
 * File:   Place.cpp
 * Author: Patrik Čada
 * 
 * Created on 6. prosince 2015, 19:10
 */

#include <vector>
#include <stddef.h>
#include <algorithm>
#include "Place.h"
#include "Types.h"
#include "CalendarEvent.h"
#include "Calendar.h"

/**
 * Vytvoření místa s počáteční kapacitou.
 * @param initCapacity
 */
Place::Place( TCapacity initCapacity, TCapacity maxCapacity ){
    this->setCapacity(initCapacity);
    this->setMaxCapacity(maxCapacity);
}


Place::~Place() {
}

/**
 * Název třídy.
 * @return 
 */
std::string Place::getName(){
    return std::string("Místo");
}

/**
 * Získání kapacity.
 * @return 
 */
TCapacity Place::getCapacity(){
    return this->capacityFront.size();
}

/**
 * Množství volné kapacity pro události.
 * @return 
 */
TCapacity Place::getFreeCapacity(){  
    CalendarEvent *empty = NULL;
    TCapacity free = std::count(this->capacityFront.begin(), this->capacityFront.end(), empty );
    return free;
}

/**
 * Získání maximální kapacity
 * @return 
 */
TCapacity Place::getMaxCapacity(){
    return this->maxCapacity;
}
        
/**
 * Nastavení kapacity.
 * @param capacity
 */
void Place::setCapacity( TCapacity capacity ){
    this->capacityFront.clear();
    this->addCapacity(capacity);    
}

/**
 * Přidání kapacity.
 * @param capacity
 */
void Place::addCapacity( TCapacity capacity ){
    this->capacityFront.insert( this->capacityFront.end(), capacity, NULL );
}

/**
 * Nastaneví maximální kapacity.
 * @param capacity
 */
void Place::setMaxCapacity( TCapacity maxCapacity ){
    this->maxCapacity = maxCapacity;
}
        
/**
 * Předání všech hran podle směru.
 * @param direct
 * @return 
 */
const TEdgeVector &Place::getEdges( Edge::Direct direct ){
    if( direct == Edge::PlaceTransition )
        return this->placeTransitionEdges;
    else
        return this->transitionPlaceEdges;
}

/**
 * Zablokování kapacity procesem.
 * @param event
 * @param count
 */
void Place::blockCapacity( CalendarEvent * event, TCapacity count ){
    CalendarEvent *empty = NULL;
    TCapacityFront::iterator i = std::find( this->capacityFront.begin(), this->capacityFront.end(), empty );
    
    TCapacity counter = 0;
    while( i != this->capacityFront.end() && counter < count ){
        *i = event;
        counter++;
        i++;
    }
}
        
/** Odebrání kapacity bez události. */
void Place::releaseCapacity( TCapacity capacity, Calendar *calendar ){
    TCapacity found = 0;
    for( TCapacityFront::iterator i = this->capacityFront.begin(); i != this->capacityFront.end() && found < capacity; i++ ){
        CalendarEvent *event = *i;
        
        //Na kapacitě je nastavena událost... zrušíme jí.
        if( event != NULL ){
            calendar->removeEvent(event);
            TCapacityFront::iterator ii = std::find( this->capacityFront.begin(), this->capacityFront.end(), event );
            for( ; ii != this->capacityFront.end(); ii++ ){
                *ii = NULL;
            }
        }
        
        this->capacityFront.erase(i);
        found++;
    }
}
        
/**
 * Odebrání kapacity událostí.
 * @param event
 */
void Place::releaseCapacity( CalendarEvent *event ){
    TCapacityFront::iterator i = std::find( this->capacityFront.begin(), this->capacityFront.end(), event );
    while( i != this->capacityFront.end() ){
        this->capacityFront.erase(i);
    }
}