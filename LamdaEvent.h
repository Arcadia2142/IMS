/* 
 * File:   TransitionEvent.h
 * Author: Patrik Čada
 *
 * Created on 7. prosince 2015, 17:40
 */

#ifndef LAMDAEVENT_H
#define LAMDAEVENT_H

#include <functional>
#include "Types.h"
#include "CalendarEvent.h"
#include "Calendar.h"

//Lamda funkce pro event.
typedef std::function<TPlaceVector *(Calendar *, CalendarEvent *)> TLamdaEvent;

/**
 * Eventy založené na lamda funkci.
 */
class LamdaEvent : public CalendarEvent {
    public:        
        LamdaEvent( TLamdaEvent lamdaEvent, Calendar *calendar );
        virtual ~LamdaEvent();
        
        /** Spuštění události. */
        TPlaceVector *run();
    protected:
        TLamdaEvent lamdaEvent;
};

#endif /* LAMDAEVENT_H */

