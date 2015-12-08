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
#include "Simulator.h"
#include "Calendar.h"
#include "Types.h"
#include "Edge.h"
#include "Transition.h"

StatisticPrinter::StatisticPrinter( Simulator *simulator ){
    this->simulator = simulator;
    this->time = this->simulator->calendar.getCurrentTime();
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
void StatisticPrinter::printStatistics( unsigned int types ){
    unsigned int parts;
    std::string column;
    
    //Vypsání transakcí:
    if( types & this->Transactions ){
        //Zobrazení transakcí:
        parts = this->charPerLines / 6;

        this->printHeader("Transakce:");
        
        column = this->createTextLine();
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
    
    //Zobrazení míst:
    if( types & this->Places ){
        this->printHeader("Místa:");
        
        parts = this->charPerLines / 7;
        column = this->createTextLine();
        this->appendColumn("ID:", column, 0, parts );
        this->appendColumn("Popis:", column, parts, parts*2 );
        this->appendColumn("Poč stav:", column, parts*3, parts );
        this->appendColumn("Kon stav:", column, parts*4, parts );
        this->appendColumn("Ček.:", column, parts*5, parts );
        this->appendColumn("Kapacita:", column, parts*6, parts );
        std::cout << column << std::endl;
        this->printLine();
        
        TPlaceVector &places = this->simulator->places;
        std::for_each( places.begin(), places.end(), [this, parts]( Place *place ){
            std::string column = this->createTextLine();
            this->appendColumn( this->convert(place->getId()), column, 0, parts );
            this->appendColumn(place->getDescription(), column, parts, parts*2 );
            this->appendColumn( this->convert( place->getDefaultCapacity() ), column, parts*3, parts );
            this->appendColumn( this->convert( place->getCapacity()), column, parts*4, parts );
            this->appendColumn( this->convert( place->getCapacity() - place->getFreeCapacity()), column, parts*5, parts );
            this->appendColumn( this->convert( place->getMaxCapacity()), column, parts*6, parts );
            std::cout << column << std::endl;
            this->printLine();
        });
        this->printLine(true);
    }
    
    //Zobrazení přechodů:
    if( types & this->Transitions ){
        this->printHeader("Přechody:");
        parts = this->charPerLines / 7;
    
        column = this->createTextLine();
        this->appendColumn("ID:", column, 0, parts );
        this->appendColumn("Popis:", column, parts, parts*2 );
        this->appendColumn("Priorita:", column, parts*3, parts );
        this->appendColumn("Šance:", column, parts*4, parts );
        this->appendColumn("Časování:", column, parts*5, parts );
        this->appendColumn("Typ časování:", column, parts*6, parts );
        std::cout << column << std::endl;
        this->printLine();
        
        
        TTransitionVector &transitions = this->simulator->transitions;
        std::for_each( transitions.begin(), transitions.end(), [this, parts]( Transition *transition ){
            std::string column = this->createTextLine();
            this->appendColumn( this->convert(transition->getId()), column, 0, parts );
            this->appendColumn( transition->getDescription(), column, parts, parts*2 );
            this->appendColumn( this->convert( transition->getPriority() ), column, parts*3, parts );
            this->appendColumn( this->convert( transition->getChance() ), column, parts*4, parts );
            this->appendColumn( this->convert( transition->getTime()), column, parts*5, parts );
            
            TTimeTypes timeType = transition->getTimeType(); 
            if( timeType == TTimeTypes::UnTime )
                this->appendColumn( "Nečasovaný" , column, parts*6, parts );
            else if( timeType == TTimeTypes::GeneratedTime )
                this->appendColumn( "Exponenciální" , column, parts*6, parts );
            else
                this->appendColumn( "Konstantní" , column, parts*6, parts );
            std::cout << column << std::endl;
            this->printLine();
        });
        this->printLine(true);
    }
    
    
    //Zobrazení hran:
    if( types & this->Edges ){
        this->printHeader("Hrany:");
        parts = this->charPerLines / 6;
        
        column = this->createTextLine();
        this->appendColumn("ID:", column, 0, parts );
        this->appendColumn("Popis:", column, parts, parts*2 );
        this->appendColumn("Místo:", column, parts*3, parts );
        this->appendColumn("Směr:", column, parts*4, parts );
        this->appendColumn("Přechod:", column, parts*5, parts );
        std::cout << column << std::endl;
        this->printLine();
        
        TEdgeVector &edges = this->simulator->edges;
        std::for_each( edges.begin(), edges.end(), [this, parts]( Edge *edge ){
            std::string column = this->createTextLine();
            this->appendColumn( this->convert(edge->getId()), column, 0, parts );
            this->appendColumn(edge->getDescription(), column, parts, parts*2 );
            this->appendColumn( this->convert( edge->getPlace()->getId() ), column, parts*3, parts );
            
            bool direct = edge->getDirect() == Edge::PlaceTransition;
            this->appendColumn( direct ? "-->" : "<--", column, parts*4, parts );
            this->appendColumn( this->convert( edge->getTransition()->getId() ), column, parts*5, parts );
            std::cout << column << std::endl;
            this->printLine();
        });
        this->printLine(true);
    }
    
    //Zobrazení přehledu:
    if( types & this->Statistics ){
        this->printHeader("Statistiky:");
        parts = this->charPerLines / 4;
        
        column = this->createTextLine();
        this->appendColumn("Přechody:", column, 0, parts );
        this->appendColumn("Místa:", column, parts, parts );
        this->appendColumn("Hrany:", column, parts*2, parts );
        this->appendColumn("Transakcí:", column, parts*3, parts );
        std::cout << column << std::endl;
        this->printLine();
        
        column = this->createTextLine();
        this->appendColumn( this->convert( this->simulator->transitions.size() ), column, 0, parts );
        this->appendColumn( this->convert( this->simulator->places.size() ), column, parts, parts );
        this->appendColumn( this->convert( this->simulator->edges.size() ), column, parts*2, parts );
        this->appendColumn( this->convert( this->infoStore.size() ), column, parts*3, parts );
        std::cout << column << std::endl;
        this->printLine(true);
        
        this->printHeader("Čas simulace:");
        column = this->createTextLine();
        this->setCenter( this->convert( *(this->time) ), column );
        std::cout << column << std::endl;
        this->printLine(true);
        
    }
    
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