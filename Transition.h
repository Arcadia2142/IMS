/* 
 * File:   Transition.h
 * Author: Patrik Čada
 *
 * Created on 6. prosince 2015, 19:11
 */

#ifndef TRANSITION_H
#define TRANSITION_H

#include "Types.h"
#include "Identificator.h"

/**
 * Třída přechodu.
 */
class Transition: public Identificator{
    friend class Simulator;
    
    public:
        /** Typ přechodu. */
        enum Type {ChanceTransition, TimeTransition};        
        virtual ~Transition();
        
        /** Název objektu */
        std::string getName();
        
        /** Priorita přechodu. */
        TPriority getPriority();
        
        /** Šance na přechod.  */
        double getChance();
        
        /** Typ času (konstantní/generátor) */
        TTimeTypes getTimeType();
        
        /** Čas přechodu */
        unsigned int getTime();
        
        /** Typ přechodu */
        Type getType();   
        
    private:
        Transition( double chance );
        Transition( TTimeTypes timeType, TTime time, TPriority priority = 1 );
        Transition( TPriority priority = 1 );
        
    private:
        /** Priorita přechodu. */
        TPriority priority = 1;
        
        /** Šance na uskutečnění přechodu. */
        double chance;
        
        /** Typ časování */
        TTimeTypes timeType = TTimeTypes::UnTime;
        
        /** Časování */
        TTime time;
        
        /** Typ přechodu */
        Type type;
        
        /** Hrany z místa do přechodu. */
        TEdgeVector placeTransactionEdges;
        
        /** Hrany z přechodu do místa. */
        TEdgeVector transactionPlaceEdges;
};

#endif /* TRANSITION_H */

