/* 
 * File:   Identificator.h
 * Author: Patrik Čada
 *
 * Created on 6. prosince 2015, 20:35
 */

#ifndef IDENTIFICATOR_H
#define IDENTIFICATOR_H

#include "Types.h"

/**
 * Abstraktní třída sloužící k identifikaci jednotlivých hran/přechodů/míst.
 * 
 */
class Identificator {
    friend class Simulator;
    
    public:
        virtual std::string getIdentificator();
        
        /** Interní ID aktuálního objektu. */
        unsigned int getId();
        
        /** Informace o jaký typ objektu se jedná. */
        virtual std::string getName() = 0;     
        
        /** Získání popisu */
        const std::string &getDescription();
        
        /** Nastavení nového popisu. */
        void setDescription( const std::string &description );
        
    protected:
        void setId( unsigned int id );
        
    protected:
        /** Popis aktuálního objektu. */
        std::string description;
        
        /** ID aktuální hrany. */
        unsigned int id;
};

#endif /* IDENTIFICATOR_H */

