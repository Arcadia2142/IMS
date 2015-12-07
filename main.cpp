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

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    Simulator *simulator = new Simulator();
    
    simulator->run();
    return 0;
}

