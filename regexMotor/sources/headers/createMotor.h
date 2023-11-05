#pragma once

#include <stdlib.h>
#include "./dynStrArray.h"
#include "./dynIntArray.h"
#include "./dynStateArray.h"

typedef struct reMotor reMotor;
struct reMotor
{
    stateListeElem* StateList;  // Contains the list of states forming the machine
    intListeElem* Actives;  // List of active states (list of ints)
};

reMotor* createMotor(char* words[], int sizeWords);
