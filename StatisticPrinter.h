/* 
 * File:   StatisticPrinter.h
 * Author: Patrik Čada
 *
 * Created on 7. prosince 2015, 19:51
 */

#ifndef STATISTICPRINTER_H
#define STATISTICPRINTER_H

#define DEFAULT_CHARS_PER_LINE 80

#include "Types.h"
#include "Calendar.h"

//Statistický informace
typedef struct SStatisticInfo TStatisticInfo;

//Uložiště statistických informací.
typedef std::vector<TStatisticInfo> TStatisticInfoStore;

/**
 * Třída starající se o ukládání a výpis statistických informací.
 */
class StatisticPrinter{
    public:
        //Interní enumerátor směru přechodu.
        enum ETransferDirect { placeTransition, transitionPlace } TTransferDirect;
        
        /** Enumerátory bitového pole pro výpis statistik. */
        enum { BaseInfo = 1, Transactions = 2, All = 3 } TInfoTypes;
        
        //Přidání transakce.
        void addTransaction( Place *place, Transition *transition, TCapacity capacity );
        
         //Přidání transakce.
        void addTransaction( Transition *transition, Place *place, TCapacity capacity );
        
        StatisticPrinter( const TTime * const time );
        virtual ~StatisticPrinter();
        
        /** Výpis statistik. */
        void printStatistics( int statistic = All );
    protected:
        //Uložení transakcí.
        void saveTransaction( Place *place, Transition *transition, TCapacity capacity, ETransferDirect direct );
        
        /** Vypsání hlavičky. */
        void printHeader( const std::string &header );
        
        /** Vytvoří nový řádek. */
        std::string createTextLine();
        
        /** Vypíše čáru. */
        void printLine( bool end = false );
        
        //Vypsání textu na střed.
        void setCenter( const std::string &text, std::string &place, unsigned int offset = 0, unsigned int maxLenght = 0 );
        
        /** Přidání sloupce do řádku. */
        void appendColumn( const std::string &text, std::string &place, unsigned int offset = 0, unsigned int maxLenght = 0 );
        
        //Konvertování číselných hodnot do stringu.
        template<typename T> std::string convert( T value);
        
    protected:
        /** Aktuální čas */
        const TTime * const time;      
        
        /** Uložiště informací o transakcích. */
        TStatisticInfoStore infoStore;
        
        /** Defaultní po počet znaků na řádek. */
        unsigned int charPerLines = DEFAULT_CHARS_PER_LINE;
};

/**
 * Struktura pro ukládání statistických informací.
 */
struct SStatisticInfo{
    Place *place;
    Transition *transition;
    StatisticPrinter::ETransferDirect direct;
    TCapacity placeCapacity;
    TCapacity edgeCapacity;
    TTime time;
};

#endif /* STATISTICPRINTER_H */

