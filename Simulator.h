/* 
 * File:   Simulator.h
 * Author: Patrik Čada
 *
 * Created on 6. prosince 2015, 19:12
 */

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "Types.h"
#include "Calendar.h"
#include "Transition.h"

/**
 * Hlavní třída simulátoru.
 */
class Simulator {
    public:
        Simulator();
        virtual ~Simulator();
        
        //Vytvoření místa.
        Place *createPlace( TCapacity initCapacity = 0, TCapacity maxCapacity = 0 );
        
        //Vytvoření přechodu.
        Transition *createTransition( double chance );
        Transition *createTransition( TTimeTypes timeType, TTime time, TPriority priority = 1 );
        Transition *createTransition( TPriority priority = 1 );
        
        //Vytvoření hrany.
        Edge *createEdge( Place *place, Transition *transition, TCapacity capacity = 1 );
        Edge *createEdge( Transition *transition, Place *place, TCapacity capacity = 1 );
        
        //Spuštění similace s zadaným maximálním časem.
        void run( TTime maxTime = DEFAULT_MAX_SIMULATION_TIME );
        
        //Zobrazení statistik.
        void printStatistics();
        
    protected:
        //Přpravení modelu ke spuštění.
        void prepareModel();
        
        //Pomocná metoda pro ukládání přechodů.
        Transition *storeTransition( Transition * transition);
        
        //Pomocná metoda pro ukládání hran.
        Edge *storeEdge( Edge * edge);
            
    private:
        Calendar calendar;
        TransactionProvider *transactionProvider;
        
        //Uložiště všechy vytvořených objektů.
        TTransitionVector transitions;
        TPlaceVector places;
        TEdgeVector edges;
        TTransactionVector transactions;
        
};

#endif /* SIMULATOR_H */

