#ifndef WINDERSTATE_H
#define WINDERSTATE_H

#include <Arduino.h>
// the state of the machine at any time
struct WinderState{
    // how 
    uint32_t totalTurns;
    uint32_t currentTurns;

};


#endif