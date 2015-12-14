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
#include "Place.h"
#include "CalendarEvent.h"
#include "StatisticPrinter.h"

/**
 * Hlavní třída simulátoru.
 */
class Simulator {
    friend class StatisticPrinter;
    
    public:
        Simulator();
        virtual ~Simulator();
        
        //Vytvoření místa.
        Place *createPlace( TCapacity initCapacity = 0, TCapacity maxCapacity = 0 );
        
        //Vytvoření přechodu.
        Transition *createTransition( double chance );
        Transition *createTransition( TTimeTypes timeType, TTime time );
        Transition *createTransition( TPriority priority = 0 );
        
        //Vytvoření hrany.
        Edge *createEdge( Place *place, Transition *transition, TCapacity capacity = 1 );
        Edge *createEdge( Transition *transition, Place *place, TCapacity capacity = 1 );
        
        //Spuštění similace s zadaným maximálním časem.
        void run( TTime maxTime = DEFAULT_MAX_SIMULATION_TIME );
        
        //Zobrazení statistik.
        void printStatistics( unsigned int types = StatisticPrinter::All );
        
    protected:
        //Přpravení modelu ke spuštění.
        void prepareModel();
        
        /** Zjištění jestly je přechod proveditelný */
        bool simTransition( Transition *transition, bool calcWithBlock = false );
        
        /** Zjištění jestly se jedná o přechody závyslé na šanci  */
        bool isChanceTransitions( const TEdgeVector &edges );
        
        /** Spuštění simulace na zadaná místa. */
        bool runPlaces( TPlaceVector *places );
        
         /** Simulace přechodu z tohoto místa. */
        bool runPlace( Place *place );
        
        /** Spuštění přechodu. */
        bool runTransition( Transition *transition );
        
        /** Provedení přechodu. */
        TPlaceVector *transferTransition( Transition *transition, CalendarEvent *event = NULL );
        
        /** Zablokování kapacity */
        void blockCapacity( Transition *transition, CalendarEvent *event );
        
        //Pomocná metoda pro ukládání přechodů.
        Transition *storeTransition( Transition * transition);
        
        //Pomocná metoda pro ukládání hran.
        Edge *storeEdge( Edge * edge);
            
    private:
        Calendar calendar;
        StatisticPrinter *statistics;
        
        //Uložiště všechy vytvořených objektů.
        TTransitionVector transitions;
        TPlaceVector places;
        TEdgeVector edges;
        
};

#endif /* SIMULATOR_H */

