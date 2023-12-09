#pragma once

#include "createMotor.h"
#include "../../../utils/headers/coloredPrints.h"

/* REMINDER OF FUNCTIONS OF SUBMODULES

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
*/

void plotMotor(reMotor* motor);
void displayState(State* state);
void nextStep(reMotor* motor, char inputChar);
void undoLastStep(reMotor* motor);
int checkAndGetFinals(reMotor* motor);
void restartMotorForNextRun(reMotor* motor);
