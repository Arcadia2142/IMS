/* 
 * File:   Transition.cpp
 * Author: Patrik Čada
 * 
 * Created on 6. prosince 2015, 19:11
 */

#include "Transition.h"

/**
 * Přechod založený na šanci.
 * @param chance
 */
Transition::Transition( double chance ){
    this->type      = Transition::ChanceTransition;
    this->chance    = chance;
}

/**
 * Přechod založený na času.
 * @param timeType
 * @param time
 * @param priority
 */
Transition::Transition( TTimeTypes timeType, TTime time, TPriority priority ){
    this->timeType  = timeType;
    this->time      = time;
    this->priority  = priority;
    this->type      = Transition::TimeTransition;
}

/**
 * Nečasovaný (okamžitý přechod)
 * @param priority
 */
Transition::Transition( TPriority priority ){
    this->timeType  = TTimeTypes::UnTime;
    this->type      = Transition::TimeTransition;
    this->priority  = priority;
}
      
Transition::~Transition() {
}

/**
 * Název třídy.
 * @return 
 */
std::string Transition::getName(){
    return std::string("Přechod");
}
        
/**
 * Priorita přechodu.
 * @return 
 */
unsigned int Transition::getPriority(){
    return this->priority;
}
        
/**
 * Šance na přechod.
 * @return 
 */
double Transition::getChance(){
    return this->chance;
}
        
/**
 * Typ času (konstantní/generátor)
 * @return 
 */
TTimeTypes Transition::getTimeType(){
    return this->timeType;
}
        
/**
 * Čas přechodu
 * @return 
 */
unsigned int Transition::getTime(){
    return this->time;
}
        
/**
 * Typ přechodu
 * @return 
 */
Transition::Type Transition::getType(){
    return this->type;
} 

/**
 * Předání všech hran podle směru.
 * @param direct
 * @return 
 */
const TEdgeVector &Transition::getEdges( Edge::Direct direct ){
    if( direct == Edge::PlaceTransition )
        return this->placeTransitionEdges;
    else
        return this->transitionPlaceEdges;
}