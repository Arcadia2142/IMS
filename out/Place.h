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
#include "Calendar.h"

//Fronta kapacity.
typedef std::vector< CalendarEvent * > TCapacityFront;

/**
 * Třída reprezentující místo.
 */
class Place: public Identificator{
    friend class Simulator;
    friend class GeneratorEvent;
    
    public:        
        /** Název objektu */
        std::string getName();
        
        /** Získání kapacity. */
        TCapacity getCapacity();
                
        /** Získání maximální kapacity */
        TCapacity getMaxCapacity();
        
        /** Získání volné kapacity (která naní přidělena události) */
        TCapacity getFreeCapacity();
        
        /** Získání počáteční kapacity. */
        TCapacity getDefaultCapacity();
        
        /** Nastavení kapacity. */
        void setCapacity( TCapacity capacity = 0 );
                
        /** Nastavení kapacity. */
        void setMaxCapacity( TCapacity maxCapacity = 0 );
        
        /** Předání všech hran podle směru.*/
        const TEdgeVector &getEdges( Edge::Direct direct );
                
    private:
        Place( TCapacity initCapacity = 0, TCapacity maxCapacity = 0 );
        
        /** Přidání kapacity. */
        void addCapacity( TCapacity capacity = 0 );
        
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
        
        /** Počáteční kapacita */
        TCapacity defaultCapacity;
        
        /** Hrany od místa k přechodu. */
        TEdgeVector placeTransitionEdges;
        
        /** Hrany od přechodu k místu. */
        TEdgeVector transitionPlaceEdges;
};

#endif /* PLACE_H */

