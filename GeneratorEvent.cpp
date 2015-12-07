/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GeneratorEvent.cpp
 * Author: Patrik Čada
 * 
 * Created on 7. prosince 2015, 1:28
 */

#include <vector>

#include "GeneratorEvent.h"
#include "Transition.h"
#include "Edge.h"
#include "Place.h"
#include "CalendarEvent.h"
#include "Calendar.h"
#include "Types.h"

GeneratorEvent::GeneratorEvent( Calendar *calendar, Transition *transition ) : CalendarEvent(calendar){
    this->transition = transition;
}

GeneratorEvent::~GeneratorEvent() {
}

/**
 * Spuštění události.
 */
TPlaceVector *GeneratorEvent::run(){
    TPlaceVector *changedPlaces = new TPlaceVector;
    
    const TEdgeVector edges = this->transition->getEdges( Edge::TransitionPlace );
    for( TEdgeVector::const_iterator i = edges.begin(); i != edges.end(); i++ ){
        Edge *edge = *(i);
        Place *place = edge->getPlace();
        
        //Nastavení kapacity místa.
        if( (place->getCapacity() + edge->getCapacity()) <= place->getMaxCapacity() || place->getMaxCapacity() == 0 ){
            place->setCapacity( place->getCapacity() + edge->getCapacity() );
            changedPlaces->push_back(place);
        }
    }
    
    //Naplánování další události.
    GeneratorEvent *event = new GeneratorEvent( this->calendar, this->transition );
    this->calendar->addEvent(event, this->calendar->calcTime(this->transition));
    return changedPlaces;
}