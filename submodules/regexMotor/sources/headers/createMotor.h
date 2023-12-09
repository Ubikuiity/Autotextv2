#pragma once

#include <stdlib.h>
#include <string.h>
#include "./dynStateArray.h"
#include "../../../utils/headers/all.h"

#define MAXINPUTSTRINGSIZE 20

typedef struct reMotor reMotor;
struct reMotor
{
    stateListeElem* StateList;  // Contains the list of states forming the machine
    intListeElem* Actives;  // List of active states (list of ints), this is the only dynamic attribute of reMotor
    intListeElem* FinalStates;  // List use to translate indexes from State to FinalStrings
    strListeElem* FinalStrings;  // List of strings that have been written when we reach the final state
    intListStack* ActivesHistory;  // History of active states
};

reMotor* createMotor(strListeElem* words);

void destroyMotor(reMotor* motor);
