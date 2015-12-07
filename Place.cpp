/* 
 * File:   Place.cpp
 * Author: Patrik Čada
 * 
 * Created on 6. prosince 2015, 19:10
 */

#include "Place.h"

/**
 * Vytvoření místa s počáteční kapacitou.
 * @param initCapacity
 */
Place::Place( TCapacity initCapacity ){
    this->setCapacity(initCapacity);
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
unsigned int Place::getCapacity(){
    return this->capacity;
}
        
/**
 * Nastavení kapacity.
 * @param capacity
 */
void Place::setCapacity( TCapacity capacity ){
    this->capacity = capacity;
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
 * Předání všech transakcí.
 * @return 
 */
const TTransactionVector &Place::getTransactions(){
    return this->transactions;
}