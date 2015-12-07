/* 
 * File:   StatisticPrinter.cpp
 * Author: Patrik Čada
 * 
 * Created on 7. prosince 2015, 19:51
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

#include "StatisticPrinter.h"
#include "Place.h"
#include "Identificator.h"

StatisticPrinter::StatisticPrinter( const TTime * const time ): time(time) {
    
}

StatisticPrinter::~StatisticPrinter() {
}

/**
 * Přidání transakce 
 * @param place
 * @param transition
 * @param capacity
 */
void StatisticPrinter::addTransaction( Place *place, Transition *transition, TCapacity capacity ){
    this->saveTransaction(place, transition, capacity, this->placeTransition);
}
        
/**
 * Přidání transakce
 * @param transition
 * @param place
 * @param capacity
 */
void StatisticPrinter::addTransaction( Transition *transition, Place *place, TCapacity capacity ){
    this->saveTransaction(place, transition, capacity, this->transitionPlace);
}

/**
 * Uložiní transakce.
 * @param place
 * @param transition
 * @param capacity
 * @param direct
 */
void StatisticPrinter::saveTransaction( Place *place, Transition *transition, TCapacity capacity, ETransferDirect direct ){
    TStatisticInfo info;
    info.direct         = direct;
    info.time           = *(this->time);
    info.place          = place;
    info.transition     = transition;
    info.placeCapacity  = place->getCapacity();
    info.edgeCapacity   = capacity;
    
    this->infoStore.push_back(info);
}

/**
 * Vypsání statistik.
 * @param types
 */
void StatisticPrinter::printStatistics( int types ){
    //Zobrazení přehledu:

    //Zobrazení míst:
    
    //Zobrazení hran:
    
    //Zobrazení přechodů:
    
    //Zobrazení transakcí:
    unsigned int parts = this->charPerLines / 6;
    
    this->printHeader("Transakce:");
    std::string column = this->createTextLine();
    this->appendColumn("Čas:", column, 0, parts );
    this->appendColumn("Místo:", column, parts, parts );
    this->appendColumn("Směr:", column, parts*2, parts );
    this->appendColumn("Akt H:", column, parts*3, parts );
    this->appendColumn("Váha:", column, parts*4, parts );
    this->appendColumn("Přechod:", column, parts*5, parts );
    std::cout << column << std::endl;
    this->printLine();
         
    //Vypsání přechodů.
    std::for_each( this->infoStore.begin(), this->infoStore.end(), [this, parts]( const TStatisticInfo &info ){
        std::string column = this->createTextLine();
        this->appendColumn(this->convert(info.time), column, 0, parts );
        this->appendColumn(this->convert(info.place->getId()), column, parts, parts );
        this->appendColumn(info.direct == this->placeTransition ? "-->" : "<--", column, parts*2, parts );
        this->appendColumn(this->convert(info.placeCapacity), column, parts*3, parts );
        this->appendColumn(this->convert(info.edgeCapacity), column, parts*4, parts );
        this->appendColumn(this->convert(info.transition->getId()), column, parts*5, parts );
        std::cout << column << std::endl;
        this->printLine();
    } );
    this->printLine(true);
}

/**
 * Zobrazení hlavičky.
 * @param header
 */
void StatisticPrinter::printHeader( const std::string &header ){
    this->printLine();
    
    std::string line = this->createTextLine();
    this->setCenter(header, line);
    std::cout << line << std::endl;    
    
    this->printLine();
}

/**
 * Vytvoří nový řádek.
 * @return 
 */
std::string StatisticPrinter::createTextLine(){
    std::string line(this->charPerLines, ' ');
    line.replace(0,1,"#");
    line.replace(this->charPerLines -1, 1, "#");
    return line;
}
        
/**
 * Vypíše čáru.
 * @param end
 */
void StatisticPrinter::printLine( bool end ){
    std::cout << std::string(this->charPerLines, '#') << std::endl;
    if( end ) std::cout << std::endl;
}
        

/**
 * Vložení textu na střed
 * @param text
 * @param place
 * @param offset
 * @param maxLenght
 */
void StatisticPrinter::setCenter( const std::string &text, std::string &place, unsigned int offset, unsigned int maxLenght ){
    unsigned int realLenght = maxLenght == 0 ? place.size() : maxLenght;
    
    if( text.size() < realLenght ){
        offset = offset + (( realLenght - text.size() ) / 2);
        realLenght = text.size();
    }
    
    place.replace(offset, realLenght, text);
}

/**
 * Vložení sloupce.
 * @param text
 * @param place
 * @param offset
 * @param maxLenght
 */
void StatisticPrinter::appendColumn( const std::string &text, std::string &place, unsigned int offset, unsigned int maxLenght ){
    unsigned int realLenght = maxLenght == 0 ? place.size() : maxLenght;
    
    place.replace(offset, 1, "#");
    this->setCenter(text, place, offset + 1, realLenght -1);
}

/**
 * Konvertace čísel do stringu.
 * @param value
 * @return 
 */
template<typename T> std::string StatisticPrinter::convert( T value){
    std::stringstream stream;
    stream << value;
    return stream.str();
}