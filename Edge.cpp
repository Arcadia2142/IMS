/* 
 * File:   Edge.cpp
 * Author: Patrik Čada
 * 
 * Created on 6. prosince 2015, 19:12
 */

#include "Edge.h"

/**
 * Hrana od místa k přechodu.
 * @param place
 * @param transition
 * @param capacity
 */
Edge::Edge( Place *place, Transition *transition, TCapacity capacity ){
    this->place         = place;
    this->transition    = transition;
    this->capacity      = capacity;
    this->direct        = Edge::PlaceTransition;
}

/**
 * Hrana od přechodu k místu.
 * @param transition
 * @param place
 * @param capacity
 */
Edge::Edge( Transition *transition, Place *place, TCapacity capacity ){
    this->place         = place;
    this->transition    = transition;
    this->capacity      = capacity;
    this->direct        = Edge::TransitionPlace;
}


Edge::~Edge() {
    
}

/**
 * Název třídy.
 * @return 
 */
std::string Edge::getName(){
    return std::string("Hrana");
}

/**
 * Předání hodnoty kapacity hrany.
 * @return 
 */
TCapacity Edge::getCapacity(){
    return this->capacity;
}

/**
 * Směr hrany.
 * @return 
 */
Edge::Direct Edge::getDirect(){
    return this->direct;
}
        
/**
 * Místo
 * @return 
 */
Place *Edge::getPlace(){
    return this->place;
}

/**
 * Přechod
 * @return 
 */
Transition *Edge::getTransition(){
    return this->transition;
}