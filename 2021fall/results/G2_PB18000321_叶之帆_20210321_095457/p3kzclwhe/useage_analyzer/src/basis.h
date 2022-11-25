#ifndef BASIC_H
#define BASIC_H

// #define DEBUG_MODE

// calculate H_fine or not
#define USE_FINE_PRIORITY 1

// output result or not
#define OUTPUT_RESULT


// -------------------
// in main.cpp

// #define RESV_FIRST

#ifndef RUN_TIME
#define RUN_TIME 1
#endif

// -------------------

#define TESTING

/*
// deprecated
#ifdef USE_LONG_SCHEDULING_CYCLE
#define SCHEDULING_CYCLE    SWAP_TIME
#else
#define SCHEDULING_CYCLE    1
#endif
*/

#ifdef DEBUG_MODE
#include <iostream>
#define DEBUG_PRINT(X)      std::cerr << "DEBUG: " #X ":" << (X) << std::endl
#define DEBUG_LOG(X)        std::cerr << "DEBUG: " << (X) << std::endl
#define DEBUG_SHOW(MSG, X)  std::cerr << "DEBUG: " << MSG << ": " << (X) << std::endl
#define DEBUG_PAUSE()       { std::cerr << "..."; while(std::cin.get() != '\n');}
#define DEBUG_DO(EXP)       EXP
#else
#define DEBUG_PRINT(X)
#define DEBUG_LOG(X)
#define DEBUG_SHOW(MSG, X)
#define DEBUG_PAUSE()
#define DEBUG_DO(EXP)
#endif // DEBUG_MODE

#endif // BASIC_H