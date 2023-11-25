#pragma once

#include <stdlib.h>
#include <string.h>
#include "./dynStrArray.h"
#include "./dynIntArray.h"
#include "./dynStateArray.h"

#define MAXINPUTSTRINGSIZE 20

typedef struct reMotor reMotor;
struct reMotor
{
    stateListeElem* StateList;  // Contains the list of states forming the machine
    intListeElem* Actives;  // List of active states (list of ints), this is the only dynamic attribute of reMotor
    intListeElem* FinalStates;  // List use to translate indexes from State to FinalStrings
    strListeElem* FinalStrings;  // List of strings that should be written when we reach final state X
};

reMotor* createMotor(char* words[], int sizeWords);

void destroyMotor(reMotor* motor);
