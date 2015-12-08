/* 
 * File:   Edge.h
 * Author: Patrik Čada
 *
 * Created on 6. prosince 2015, 19:12
 */

#ifndef EDGE_H
#define EDGE_H

#include "Types.h"
#include "Identificator.h"

/**
 * Třída reprezentující hranu.
 */
class Edge : public Identificator{
    friend class Simulator;
    
    public:
        /** Směr přechodu. */
        enum Direct{ PlaceTransition, TransitionPlace};
        
        virtual ~Edge();
        
        /** Název objektu */
        std::string getName();
        
        /** Kapacita hrany */
        TCapacity getCapacity();
        
        /** Směr */
        Direct getDirect();
        
        /** Místo */
        Place *getPlace();
        
        /** třechod */
        Transition *getTransition();
        
    private:
        /** Hrana z místa do přechodu. */
        Edge( Place *place, Transition *transition, TCapacity capacity = 1 );
        
        /** Hrana z přechodu do místa. */
        Edge( Transition *transition, Place *place, TCapacity capacity = 1 );
        
    private:
        unsigned int capacity;
        Direct direct;
        
        Place *place;
        Transition *transition;
};

#endif /* EDGE_H */

