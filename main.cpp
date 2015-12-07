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
#include "Simulator.h"
#include "Types.h"
#include "Place.h"
#include "Identificator.h"
#include "Transition.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {   
    Simulator *simulator = new Simulator();
    
    /**
     * Simulace "Krava / auto"
     * ims-demo1.pdf slide 23/24.
     */
    
    //Kráva / dojička:
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
    Place *konvice = simulator->createPlace();
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
    Transition *exp15min = simulator->createTransition( 15.0/60.0 );
    exp15min->setDescription("Exp 15min");
    simulator->createEdge( p4, exp15min );
    simulator->createEdge(exp15min, p6);
    
    //5
    Transition *exp8min = simulator->createTransition( 8.0/60.0 );
    exp8min->setDescription("Exp 8min");
    simulator->createEdge( p5, exp8min );
    simulator->createEdge(exp8min, p6);
    
    //6
    Transition *t6 = simulator->createTransition();
    t6->setDescription("T6");
    simulator->createEdge(p6, t6);
    simulator->createEdge(t6, p0);
    simulator->createEdge(t6, konvice);
    simulator->createEdge(t6, dojicky);
    
    
    
    simulator->run(1000);
    simulator->printStatistics();
    delete simulator;
    return 0;
}

