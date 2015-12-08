/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GeneratorEvent.h
 * Author: Patrik Čada
 *
 * Created on 7. prosince 2015, 1:28
 */

#ifndef GENERATOREVENT_H
#define GENERATOREVENT_H

#include "CalendarEvent.h"

/**
 * Event generátoru( Vytvářející kapacitu. )
 */
class GeneratorEvent : public CalendarEvent{
    public:
        GeneratorEvent( Calendar *calendar, Transition *transition );
        virtual ~GeneratorEvent();
   
        TPlaceVector *run();
        
    protected:
        Transition * transition;
};

#endif /* GENERATOREVENT_H */

