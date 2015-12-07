/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TransitionEvent.cpp
 * Author: Patrik Čada
 * 
 * Created on 7. prosince 2015, 17:40
 */

#include <algorithm>
#include "LamdaEvent.h"
#include "CalendarEvent.h"

/**
 * 
 * @param transition
 * @param calendar
 */
LamdaEvent::LamdaEvent( TLamdaEvent lamdaEvent, Calendar *calendar ) : CalendarEvent(calendar) {
    this->lamdaEvent = std::move(lamdaEvent);
}

LamdaEvent::~LamdaEvent() {
    
}

/**
 * Spuštění lamda eventu.
 * @return 
 */
TPlaceVector *LamdaEvent::run(){
    return this->lamdaEvent( this->calendar, this );
}