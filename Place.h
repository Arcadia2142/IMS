/* 
 * File:   Place.h
 * Author: Patrik Čada
 *
 * Created on 6. prosince 2015, 19:10
 */

#ifndef PLACE_H
#define PLACE_H

#include "Types.h"
#include "Edge.h"
#include "Identificator.h"
#include "CalendarEvent.h"

//Fronta kapacity.
typedef std::vector< CalendarEvent * > TCapacityFront;

/**
 * Třída reprezentující místo.
 */
class Place: public Identificator{
    friend class Simulator;
    
    public:        
        /** Název objektu */
        std::string getName();
        
        /** Získání kapacity. */
        TCapacity getCapacity();
                
        /** Získání maximální kapacity */
        TCapacity getMaxCapacity();
        
        /** Nastavení kapacity. */
        void setCapacity( TCapacity capacity = 0 );
        
        /** Přidání kapacity. */
        void addCapacity( TCapacity capacity = 0 );
        
        /** Nastavení kapacity. */
        void setMaxCapacity( TCapacity maxCapacity = 0 );
        
        /** Předání všech hran podle směru.*/
        const TEdgeVector &getEdges( Edge::Direct direct );
                
    private:
        Place( TCapacity initCapacity = 0, TCapacity maxCapacity = 0 );
        
        /** Získání volné kapacity (která naní přidělena události) */
        TCapacity getFreeCapacity();
        
        /** Zablokování kapacity procesem */
        void blockCapacity( CalendarEvent * event, TCapacity count );
        
        /** Odebrání kapacity bez události. */
        void releaseCapacity( TCapacity capacity, Calendar *calendar );
        
        /** Odebrání kapacity událostí. */
        void releaseCapacity( CalendarEvent *event );
                
        virtual ~Place();
        
    private:
        /** Fronta kapacity. */
        TCapacityFront capacityFront;
        
        /** Kapacita místa, 0 = nekonečno */
        TCapacity maxCapacity;
        
        /** Hrany od místa k přechodu. */
        TEdgeVector placeTransitionEdges;
        
        /** Hrany od přechodu k místu. */
        TEdgeVector transitionPlaceEdges;
};

#endif /* PLACE_H */

