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
#include "LamdaEvent.h"
#include "CalendarEvent.h"
#include "StatisticPrinter.h"

Simulator::Simulator(){
    this->statistics = new StatisticPrinter( this );
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
    
    delete this->statistics;
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
 * @return 
 */
Transition *Simulator::createTransition( TTimeTypes timeType, TTime time ){
    Transition *transition = new Transition(timeType, time);
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
    transition->transitionPlaceEdges.push_back(edge);
    
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
        const TEdgeVector edgeVector = transition->getEdges( Edge::PlaceTransition );
        
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
        if( !(state = this->runPlaces( &(this->places) ))){
            TPlaceVector *eventPlaces = calendar.runEvent(maxTime);
            
            if( eventPlaces != NULL){
                if((state = (eventPlaces->size() > 0)) ){
                    this->runPlaces( eventPlaces );
                }
                
                delete eventPlaces;
            }
            
            state = state || this->runPlaces( &(this->places) );
        }
    }while(state);
   
    
}
        
/**
 * Spuštění simulace na zadaná místa.
 * @param places
 * @return 
 */
bool Simulator::runPlaces( TPlaceVector *places ){
    bool state = false;
    std::for_each( places->begin(), places->end(), [&state, this](Place * const place){
        state = state | this->runPlace(place);
    } );
    
    return state;
}

/**
 * Simulace přechodu z tohoto místa.
 * @param place
 * @return 
 */
bool Simulator::runPlace( Place *place ){
    bool state = false;
    
    if( place->getCapacity() > 0 ){
        const TEdgeVector &placeEdges = place->getEdges( Edge::PlaceTransition );
        
        //Přechody na principu šance
        if( this->isChanceTransitions(placeEdges) ){
            double chance = this->calendar.getNorm();
            
            std::all_of( placeEdges.begin(), placeEdges.end(), [&chance, &state, this](Edge *edge){
                Transition *transition = edge->getTransition();
                
                if( transition->chance >= chance ){
                    if( this->simTransition(transition) ){
                        state = this->runTransition(transition);
                    }
                    
                    return false;
                }
                
                return true;
            } );
            
            return state;
        }
        
        //Zkopírování pouze proveditelných přechodů.
        TEdgeVector usableEdges;
        std::for_each( placeEdges.begin(), placeEdges.end(), [place, &usableEdges]( Edge * const edge ){
            if( place->getCapacity() >= edge->getCapacity() )
                usableEdges.push_back(edge);
        } );
        
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
                TPriorityVector priorityVector = std::make_pair(priority, TEdgeVector());
                priorityVector.second.push_back(edge);
                priorityMap.insert( it, priorityVector );
            }else
                (*it).second.push_back(edge);
        });
        
        std::sort( priorityMap.begin(), priorityMap.end(), []( const TPriorityVector &vector1, const TPriorityVector &vector2 ){
            return vector1.first > vector2.first;
        } );
       
        //Spuštíme simulaci od nejvyší priority
        std::all_of(priorityMap.begin(), priorityMap.end(), [&state, place, this]( TPriorityVector &pair ){
            TEdgeVector &edgeVector = pair.second;

            //Spustění jednotlivých hran
            std::all_of( edgeVector.begin(), edgeVector.end(), [&state, place, this]( Edge * edge ){
                Transition *transition = edge->getTransition();
                state = state | (this->simTransition(transition) && this->runTransition(transition));       
                return place->getCapacity() > 0;
            } );

            return place->getCapacity() > 0;
        });
    }
    
    return state;
}

/**
 * Spuštění přechodu.
 * @param transition
 * @return 
 */
bool Simulator::runTransition( Transition *transition ){
    //Časový přechody na principu kalendáře
    if( transition->type == Transition::TimeTransition && transition->timeType != TTimeTypes::UnTime ){
        if( this->simTransition(transition, true) ){
            //Lamda event vykonaný podle časovače v kalendáři.
            TLamdaEvent lamda = [transition, this](Calendar *calendar, CalendarEvent *event){
                return this->transferTransition(transition, event);
            };
            
            //Přidání eventu
            LamdaEvent *event = new LamdaEvent(lamda, &(this->calendar));
            this->blockCapacity(transition, event);
            this->calendar.addEvent(event, this->calendar.calcTime(transition));
            return true;
        }
        
        return false;
    }
    
    this->transferTransition(transition);    
    return true;
}

/**
 * Přenos kapacity mezi místem -> přechodem -> místem.
 * @param transition
 * @param event
 */
TPlaceVector *Simulator::transferTransition( Transition *transition, CalendarEvent *event ){
    //Provedení okamžitého přechodu.
    const TEdgeVector &edgesIn = transition->getEdges( Edge::PlaceTransition );
    const TEdgeVector &edgesOut = transition->getEdges( Edge::TransitionPlace );
    
    TPlaceVector *places = event == NULL ? NULL : new TPlaceVector;
    
    //Hran z místa do přechodu.
    std::for_each( edgesIn.begin(), edgesIn.end(), [this, event, transition]( Edge *edge ){
        Place *place = edge->getPlace();
        TCapacity capacity = edge->getCapacity();
        
        //Logování transakce
        this->statistics->addTransaction( place, transition, capacity );
        
        //Provedení transakce.
        if( event == NULL ){
            place->releaseCapacity( capacity , &(this->calendar) );
        }else{
            place->releaseCapacity(event);
        }
    });
    
    
    //Hrany z přechodu do místa.
    std::for_each( edgesOut.begin(), edgesOut.end(), [places, transition, this]( Edge *edge ){
        Place *place = edge->getPlace();
        TCapacity capacity = edge->getCapacity();
        
        //Logování transakce
        this->statistics->addTransaction( transition, place, capacity );
        
        place->addCapacity(capacity); 
        if(places != NULL){
            places->push_back(place);
        }
    });
    
    return places;
}

/**
 * Zablokování kapacity míst.
 * @param transition
 * @param event
 */
void Simulator::blockCapacity( Transition *transition, CalendarEvent *event ){
    const TEdgeVector &edges = transition->getEdges( Edge::PlaceTransition );
    
    std::for_each( edges.begin(), edges.end(), [event]( Edge *edge ){
        Place *place = edge->getPlace();
        place->blockCapacity(event, edge->getCapacity());
    });
}

/**
 * Zjištění jestly je přechod proveditelný
 * @param transition
 * @return 
 */
bool Simulator::simTransition( Transition *transition, bool calcWithBlock ){
    const TEdgeVector &inputEdges   = transition->getEdges(Edge::PlaceTransition);
    const TEdgeVector &outputEdges  = transition->getEdges(Edge::TransitionPlace);
    
    //Kontrola jestly jsou hrany z míst proveditelné.
    bool canTransfer = true;
    std::all_of( inputEdges.begin(), inputEdges.end(), [&canTransfer, calcWithBlock]( Edge *edge ){
        Place *place = edge->getPlace();
        TCapacity freeCapacity = calcWithBlock ? place->getFreeCapacity() : place->getCapacity(); 
        canTransfer = canTransfer & ( freeCapacity >= edge->getCapacity());
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
    
    return canTransfer;
}

/**
 * Zjištění jestly se jedná o přechody závyslé na šanci.
 * @param edges
 * @return 
 */
bool Simulator::isChanceTransitions( const TEdgeVector &edges ){
    double chance = 0.0;
    
    bool state = std::all_of( edges.begin(), edges.end(), [&chance]( Edge *edge ){
        Transition *transition = edge->getTransition();
        if( transition->getType() == Transition::ChanceTransition ){
            chance = chance + transition->getChance();            
            return true;
        }
        
        return false;
    });
    
    return state;
}

/**
 * Vypsání statistik simulátoru.
 * @param types
 */
void Simulator::printStatistics( unsigned int types ){
    this->statistics->printStatistics( types );
}