/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Patrik Čada
 *
 * Created on 5. prosince 2015, 16:36
 */

#include <cstdlib>
#include <iosfwd>
#include <string>
#include <iostream>
#include "Simulator.h"
#include "Types.h"
#include "Place.h"
#include "Identificator.h"
#include "Transition.h"
#include "StatisticPrinter.h"

using namespace std;

//Prototyp simulace
void cowSimulation( Simulator *simulator );
void printHelp();

/*
 * 
 */
int main(int argc, char** argv) {   
    TTime simTime = DEFAULT_MAX_SIMULATION_TIME;
    unsigned int statistics = StatisticPrinter::All;
    
    //Zpracování argumentů.
    char modificator = '\0';
    for( int i = 1; i < argc; i++ ){
        //Help
        if( string(argv[i]).compare("--help") == 0){
            printHelp();
            return 0;
        }
        
        
        //Parsing argumentů.
        if( argv[i][0] == '-' ){
            modificator = argv[i][1];
            continue;
        }else if( modificator != '\0' ){ 
            char mod = modificator;
            modificator = '\0'; 
           
            if( mod == 't' ){
                simTime = atoi(argv[i]);
                continue;
            }else if( mod == 's' ){
                statistics = atoi(argv[i]);
                continue;
            }
        }
        
        //Chybné
        cout << "Byly zadány chybné parametry, zadejte --help pro informace" << endl;
        return 1;
    }
    
    Simulator *simulator = new Simulator();
    
    //Loadování simulačního modelu.
    cowSimulation(simulator);
    
    simulator->run(simTime);
    simulator->printStatistics( statistics );
    
    delete simulator;
    return 0;
}

/**
 * Náápověda :D
 */
void printHelp(){
    cout << "####### Nápověda k simulátoru petriho sítí #########" << endl;
    cout << "© Patrik Čada (xcadap00) v rámci projektu do IMS" << endl;
    cout << "Dostupné parametry:" << endl;
    cout << " -t 1000    - Nastaví maximální čas simulace na 1000." << endl;
    cout << " -s 4       - Zobrazí informace o místech v modelu." << endl;
    cout << "Parametr s je součet čísel informací které hledáte( princip bitového pole)" << endl;
    cout << "Zobrazitelné informace:" << endl;
    cout << "0 - Nezobrazovat nic" << endl;
    cout << "1 - Přehledová statistika" << endl;
    cout << "2 - Provedené transakce" << endl;
    cout << "4 - Informace o místech" << endl;
    cout << "8 - Informace o přechodech" << endl;
    cout << "16 - Informace o hranách" << endl;
    cout << "31 - Zobrazit vše (výchozí)" << endl;
    cout << "Příklad pro zobrazení informací o místech, přechodech, hranách, přehledové statistice" << endl;
    cout << "Za čas simulace 1000:" << endl;
    cout << "./simulator -t 1000 -p 29" << endl;
}

/**
* Simulace "Krava / Dojička / auto"
* ims-demo1.pdf slide 23/24.
*/
void cowSimulation( Simulator *simulator ){ 
    //Kráva / dojička (slide 23):
    
    //0
    Place *p0 = simulator->createPlace(100);
    p0->setDescription("P0");
    
    //1
    Place *p1 = simulator->createPlace();
    p1->setDescription("P1");
    
    //2
    Place *dojicky = simulator->createPlace(5);
    dojicky->setDescription("Dojicky");
    
    //3
    Place *p3 = simulator->createPlace();
    p3->setDescription("P3");
    
    //4
    Place *p4 = simulator->createPlace();
    p4->setDescription("P4");
    
    //5
    Place *p5 = simulator->createPlace();
    p5->setDescription("P5");
    
    //6
    Place *p6 = simulator->createPlace();
    p6->setDescription("P6");
    
    //7
    Place *konvice = simulator->createPlace(0,20);
    konvice->setDescription("Konvice");
    
    //0
    Transition *exp15h = simulator->createTransition(TTimeTypes::GeneratedTime, 15.0);
    exp15h->setDescription("Exp 15h");
    simulator->createEdge(p0, exp15h);
    simulator->createEdge(exp15h, p1);
    
    //1
    Transition *t1 = simulator->createTransition();
    t1->setDescription("T1");
    simulator->createEdge(p1, t1);
    simulator->createEdge(dojicky, t1);
    simulator->createEdge(t1, p3);
    
    //2
    Transition *chg1 = simulator->createTransition(0.1);
    chg1->setDescription("Šance 10%");
    simulator->createEdge(p3, chg1);
    simulator->createEdge(chg1, p4 );
    
    //3
    Transition *chg2 = simulator->createTransition(0.9);
    chg2->setDescription("Šance 90%");
    simulator->createEdge(p3, chg2);
    simulator->createEdge(chg2, p5 );
    
    //4
    Transition *exp15min = simulator->createTransition( TTimeTypes::GeneratedTime, 15.0/60.0 );
    exp15min->setDescription("Exp 15min");
    simulator->createEdge( p4, exp15min );
    simulator->createEdge(exp15min, p6);
    
    //5
    Transition *exp8min = simulator->createTransition( TTimeTypes::GeneratedTime, 8.0/60.0 );
    exp8min->setDescription("Exp 8min");
    simulator->createEdge( p5, exp8min );
    simulator->createEdge(exp8min, p6);
    
    //6
    Transition *t6 = simulator->createTransition();
    t6->setDescription("T6");
    simulator->createEdge( p6, t6 );
    simulator->createEdge( t6, p0 );
    simulator->createEdge( t6, konvice );
    simulator->createEdge( t6, dojicky );
    
    //Auto - Slide 24
    //7
    Place *auta = simulator->createPlace(2);
    auta->setDescription("Auta");
    
    //8
    Place *ramp = simulator->createPlace(1);
    ramp->setDescription("Rampa");
    
    //9
    Place *onRamp = simulator->createPlace();
    onRamp->setDescription("Na rampě");
    
    //10
    Place *capacity = simulator->createPlace();
    capacity->setDescription("Kapacita");
    
    //11
    Place *p7 = simulator->createPlace();
    p7->setDescription("P7");
    
    //12
    Place *p8 = simulator->createPlace();
    p8->setDescription("P8");
    
    //13
    Place *p9 = simulator->createPlace();
    p9->setDescription("P9");
    
    
    //9
    Transition *t7 = simulator->createTransition();
    t7->setDescription( "T7" );
    simulator->createEdge(auta, t7);
    simulator->createEdge(ramp, t7);
    simulator->createEdge(t7, capacity, 20);
    simulator->createEdge(t7, onRamp);
    
    //10
    Transition *t8 = simulator->createTransition((TPriority)1);
    t8->setDescription( "Priorita 1" );
    simulator->createEdge(capacity, t8);
    simulator->createEdge(onRamp, t8);
    simulator->createEdge(t8, p7);
    
    //11
    Transition *t9 = simulator->createTransition();
    t9->setDescription( "T9" );
    simulator->createEdge(onRamp, t9);
    simulator->createEdge(t9, ramp);
    simulator->createEdge(t9, p8);
    
    //12
    Transition *t10 = simulator->createTransition();
    t10->setDescription( "T10" );
    simulator->createEdge(p7, t10);
    simulator->createEdge(konvice, t10);
    simulator->createEdge(t10, p9);
    
    //13
    Transition *t11 = simulator->createTransition( TTimeTypes::ConstantTime, 1.0 );
    t11->setDescription( "1h" );
    simulator->createEdge(p8, t11);
    simulator->createEdge(t11, auta);
    
    //14
    Transition *t12 = simulator->createTransition( TTimeTypes::ConstantTime, 1.5/60.0 );
    t12->setDescription( "1,5min" );
    simulator->createEdge(p9, t12);
    simulator->createEdge(t12, onRamp);
};

