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
        
        /** Nastavení kapacity. */
        void setCapacity( TCapacity capacity = 0 );
        
        /** Předání všech hran podle směru.*/
        const TEdgeVector &getEdges( Edge::Direct direct );
        
        /** Předání všech transakcí. */
        const TTransactionVector &getTransactions();
        
    private:
        Place( TCapacity initCapacity = 0 );
        virtual ~Place();
        
    private:
        /** Kapacita místa, 0 = nekonečno */
        TCapacity capacity = 0;
        
        /** Hrany od místa k přechodu. */
        TEdgeVector placeTransitionEdges;
        
        /** Hrany od přechodu k místu. */
        TEdgeVector transitionPlaceEdges;
        
        /** Transakce uložené v aktuálním místě */
        TTransactionVector transactions;
};

#endif /* PLACE_H */

