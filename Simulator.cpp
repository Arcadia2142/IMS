/* 
 * File:   Simulator.cpp
 * Author: Patrik Čada
 * 
 * Created on 6. prosince 2015, 19:12
 */

#include <vector>
#include <algorithm>

#include "Simulator.h"
#include "Transition.h"
#include "Place.h"
#include "Edge.h"
#include "Types.h"
#include "Identificator.h"
#include "Calendar.h"
#include "GeneratorEvent.h"

Simulator::Simulator(){
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
    
}

/**
 * Vytvoření místa
 * @param initCapacity
 * @param maxCapacity
 * @return 
 */
Place *Simulator::createPlace( TCapacity initCapacity, TCapacity maxCapacity ){
    Place *place = new Place(initCapacity, maxCapacity);
    place->setId( this->places.size() );
    this->places.push_back(place);    
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
    transition->setId( this->transitions.size() );
    this->transitions.push_back(transition);
    return transition;
}
        
//Vytvoření hrany
Edge *Simulator::createEdge( Place *place, Transition *transition, TCapacity capacity ){
    Edge *edge = new Edge( place, transition, capacity );
    
    place->placeTransitionEdges.push_back(edge);
    transition->placeTransitionEdges.push_back(edge);
    
    return this->storeEdge(edge);
}

Edge *Simulator::createEdge( Transition *transition, Place *place, TCapacity capacity ){
    Edge *edge = new Edge( transition, place, capacity );
    
    place->transitionPlaceEdges.push_back(edge);
    transition->placeTransitionEdges.push_back(edge);
    
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
    //Nalezení přechodů které nemají vstupní hrany(generátorů).
    for( TTransitionVector::iterator i = this->transitions.begin(); i != this->transitions.end(); i++ ){
        Transition *transition = *(i);
        const TEdgeVector edgeVector = transition->getEdges( Edge::TransitionPlace );
        
        if( edgeVector.size() == 0 ){
            GeneratorEvent *event = new GeneratorEvent( &(this->calendar), transition);
            this->calendar.addEvent(event, this->calendar.calcTime(transition));
        }
    }
}

/**
 * Spuštění simulátoru.
 * @param maxTime
 */
void Simulator::run( TTime maxTime ){
    this->prepareModel();
    bool state = true;
    
    //Běh simulace.
    do{
        if( !(state = this->simPlaces( &(this->places) ))){
            TPlaceVector *eventPlaces = calendar.runEvent(maxTime);
            
            if( state = (eventPlaces->size() > 0) ){
                this->simPlaces( eventPlaces );
            }
        }
    }while(state);
}
        
/**
 * Spuštění simulace na zadaná místa.
 * @param places
 * @return 
 */
bool Simulator::simPlaces( TPlaceVector *places ){
    bool state = false;
    std::for_each( places->begin(), places->end(), [&state, this](Place * const place){
        state = state | this->simPlace(place);
    } );
    
    return state;
}

/**
 * Simulace přechodu z tohoto místa.
 * @param place
 * @return 
 */
bool Simulator::simPlace( Place *place ){
    bool state = false;
    
    if( place->getCapacity() > 0 ){
        const TEdgeVector &placeEdges = place->getEdges( Edge::PlaceTransition );
        
        //Zkopírování pouze proveditelných přechodů.
        TEdgeVector usableEdges = TEdgeVector(placeEdges.size());
        auto it = std::copy_if( placeEdges.begin(), placeEdges.end(), usableEdges.begin(), [place]( Edge * const edge ){
            return place->getCapacity() >= edge->getCapacity();
        } );
        usableEdges.resize( std::distance(usableEdges.begin(), it) );
        
       //Seřadíme hrany podle priority přechodu.
       std::vector<TPriorityVector> priorityMap;
       std::for_each( usableEdges.begin(), usableEdges.end(), [&priorityMap]( Edge * const edge ){
           TPriority priority = edge->getTransition()->getPriority();
           
           //Vyhledání priority ve vektoru.
           auto it = std::find_if( priorityMap.begin(), priorityMap.end(), [priority]( const TPriorityVector &pair){
               return pair.first == priority;
           });
           
           //Vložení nové priority.
           if( it == priorityMap.end() ){
               it = priorityMap.end();
               priorityMap.insert( it, std::make_pair(priority, TEdgeVector()) );
           }
           
           //Přidání hrany.
           (*it).second.push_back(edge);
       });
       std::stable_sort( priorityMap.rbegin(), priorityMap.rend() );
       
       //Spuštíme simulaci od nejvyší priority
       std::all_of(priorityMap.begin(), priorityMap.end(), [&state, place, this]( TPriorityVector &pair ){
           TEdgeVector &edgeVector = pair.second;
           
           //Spustění jednotlivých hran
           std::all_of( edgeVector.begin(), edgeVector.end(), [&state, place, this]( Edge * edge ){
               Transition *transition = edge->getTransition();
               state = state | this->simTransition(transition);       
               return place->getCapacity() > 0;
           } );
           
           return place->getCapacity() > 0;
       });
    }
    
    return state;
}

/**
 * Spuštění simulace přechodu.
 * @param transition
 * @return 
 */
bool Simulator::simTransition( Transition *transition ){
    const TEdgeVector &inputEdges   = transition->getEdges(Edge::PlaceTransition);
    const TEdgeVector &outputEdges  = transition->getEdges(Edge::TransitionPlace);
    
    //Kontrola jestly jsou hrany z míst proveditelné.
    bool canTransfer = true;
    std::all_of( inputEdges.begin(), inputEdges.end(), [&canTransfer]( Edge *edge ){
        Place *place = edge->getPlace();
        canTransfer = canTransfer & (place->getCapacity() >= edge->getCapacity());
        return canTransfer;
    });
    
    if( !canTransfer )
        return false;
    
    //Kontrola kapacity míst za přechodem. 
    std::all_of(outputEdges.begin(), outputEdges.end(), [&canTransfer]( Edge *edge ){
        Place *place = edge->getPlace();
        canTransfer = canTransfer & ( 
            place->getMaxCapacity() == 0 || 
            (edge->getCapacity() + place->getCapacity()) <= place->getMaxCapacity()
        );
            
        return canTransfer;
    });
    
    if( !canTransfer )
        return false;
    
    //Provedení přechodu.
    //if( transition )
    // TODO: zde jsem zkončil.
    
    return true;
}