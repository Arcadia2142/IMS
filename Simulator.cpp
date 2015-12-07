/* 
 * File:   Simulator.cpp
 * Author: Patrik Čada
 * 
 * Created on 6. prosince 2015, 19:12
 */

#include <vector>

#include "Simulator.h"
#include "Transition.h"
#include "Place.h"
#include "Edge.h"
#include "Transaction.h"
#include "Types.h"
#include "Identificator.h"
#include "Calendar.h"
#include "TransactionProvider.h"

Simulator::Simulator(){
    this->transactionProvider = new TransactionProvider( 
        &(this->transactions),
        this->calendar.getCurrentTime()
    );
}

/**
 * Dealokování alokovaných objektů.
 */
Simulator::~Simulator() {
    //Dealokace hran
    for(TEdgeVector::iterator i = this->edges.begin(); i != this->edges.end(); i++){
        delete *i;
    }
    
    //Dealokace míst
    for(TPlaceVector::iterator i = this->places.begin(); i != this->places.end(); i++){
        delete *i;
    }
    
    //Dealokace přechodů
    for(TTransitionVector::iterator i = this->transitions.begin(); i != this->transitions.end(); i++){
        delete *i;
    }
    
    //Dealokace transakcí
    for(TTransactionVector::iterator i = this->transactions.begin(); i != this->transactions.end(); i++){
        delete *i;
    }
    
    delete this->transactionProvider;
}

/**
 * Vytvoření místa
 * @param initCapacity
 * @param maxCapacity
 * @return 
 */
Place *Simulator::createPlace( TCapacity initCapacity, TCapacity maxCapacity ){
    Place *place = new Place(maxCapacity);
    place->setId( this->places.size() );
    this->places.push_back(place);
    
    //Vložení počátečních transakcí.
    for( TCapacity i = 0; i < initCapacity; i++ ){
        Transaction *transaction = this->transactionProvider->createTransaction(place);
        place->transactions.push_back(transaction);
    }
    
    return place;
}
        
/**
 * Vytvoření přechodu závyslého na šanci.
 * @param chance
 * @return 
 */
Transition *Simulator::createTransition( double chance ){
    Transition *transition = new Transition(chance);
    return this->storeTransition(transition);
}

/**
 * Vytvoření přechodu závyslého na čase.
 * @param timeType
 * @param time
 * @param priority
 * @return 
 */
Transition *Simulator::createTransition( TTimeTypes timeType, TTime time, TPriority priority ){
    Transition *transition = new Transition(timeType, time, priority);
    return this->storeTransition(transition);
}

/**
 * Vytvoření přechodu závyslého na prioritě.
 * @param priority
 * @return 
 */
Transition *Simulator::createTransition( TPriority priority ){
    Transition *transition = new Transition(priority);
    return this->storeTransition(transition);
}

/**
 * Pomocná metoda pro ukládání přechodů.
 * @param transition
 * @return 
 */
Transition *Simulator::storeTransition( Transition * transition){
    transition->setId( this->transactions.size() );
    this->transitions.push_back(transition);
    return transition;
}
        
//Vytvoření hrany
Edge *Simulator::createEdge( Place *place, Transition *transition, TCapacity capacity ){
    Edge *edge = new Edge( place, transition, capacity );
    
    place->placeTransitionEdges.push_back(edge);
    transition->placeTransactionEdges.push_back(edge);
    
    return this->storeEdge(edge);
}

Edge *Simulator::createEdge( Transition *transition, Place *place, TCapacity capacity ){
    Edge *edge = new Edge( transition, place, capacity );
    
    place->transitionPlaceEdges.push_back(edge);
    transition->transactionPlaceEdges.push_back(edge);
    
    return this->storeEdge(edge);
}

/**
 * Pomocná metoda pro ukládání hran.
 * @param edge
 * @return 
 */
Edge *Simulator::storeEdge( Edge *edge ){
    edge->setId( this->edges.size() );
    this->edges.push_back( edge );
    return edge;
}

/**
 * Připravení modelu ke spuštění.
 */
void Simulator::prepareModel(){
    
}

/**
 * Spuštění simulátoru.
 * @param maxTime
 */
void Simulator::run( TTime maxTime ){
    this->prepareModel();
}