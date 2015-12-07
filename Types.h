/* 
 * File:   Types.h
 * Author: Patrik Čada
 *
 * Created on 6. prosince 2015, 19:49
 */

#ifndef TYPES_H
#define TYPES_H

#define DEFAULT_MAX_SIMULATION_TIME 1000

#include <stddef.h>
#include <string>
#include <vector>
#include <utility>

//Používaný třídy v projektu.
class Identificator;
class Calendar;
class CalendarEvent;
class LamdaEvent;
class Simulator;
class Place;
class Transition;
class StatisticPrinter;
class Edge;

/** Kapacita místa/hrany */
typedef unsigned int TCapacity;

/** Priorita přechodů. */
typedef unsigned int TPriority;

/** Typy časů. */
typedef enum{ UnTime, ConstantTime, GeneratedTime } TTimeTypes;

/** Pole hran */
typedef std::vector<Edge *> TEdgeVector;

/** Time type in simulator. */
typedef double TTime;

/** Statistický pár času s objektem přez který prochází. */
typedef std::pair<TTime, Identificator *> TTimeIdentificatorPair;

/** Vektor s časy. */
typedef std::vector< TTimeIdentificatorPair > TTimeIdentificators;

/** Pát priority s vektorem. */
typedef std::pair<TPriority, TEdgeVector> TPriorityVector;

//Pomocné typy vektorů
typedef std::vector<Transition *> TTransitionVector;
typedef std::vector<Place *> TPlaceVector;
#endif /* TYPES_H */

