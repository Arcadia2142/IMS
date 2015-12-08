/* 
 * File:   Edge.cpp
 * Author: Patrik Čada
 * 
 * Created on 6. prosince 2015, 19:12
 */

#include "Identificator.h"
#include <sstream>

/**
 * ID aktuálního objektu.
 * @return 
 */
unsigned int Identificator::getId(){
    return this->id;
}

/**
 * Nastavení ID
 * @param id
 */
void Identificator::setId( unsigned int id ){
    this->id = id;
}

/**
 * Popis aktuálního objektu.
 * @return 
 */
const std::string &Identificator::getDescription(){
    return this->description;
}

/**
 * Nastavení uživatelského popisu.
 * @param description
 */
void Identificator::setDescription( const std::string &description ){
    this->description = description;
}

/**
 * Navrácení čitelného identifikátoru.
 * @return 
 */
std::string Identificator::getIdentificator(){
    std::stringstream stream;
    
    stream << this->getName() << ":";
    stream << this->getId() << ":";
    stream << this->getDescription();
    
    return std::string(  stream.str()  );
}