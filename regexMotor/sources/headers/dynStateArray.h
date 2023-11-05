#pragma once

#include <stdlib.h>
#include <stdio.h>

struct stateListeElem;

typedef struct State State;
struct State
{
    int Id;
    struct stateListeElem* outs;  // The states accessible from this state
    char condition;  // Thte condition (letter here) required to enter the state
};

typedef struct stateListeElem stateListeElem;
struct stateListeElem
{
    State* value;  // Pointer to the string stored
    stateListeElem* next;
};

State* createState(int Id, stateListeElem* outs, char condition);

stateListeElem* createStateListWithFirstElem(State* val);

State* getStateListeValue(stateListeElem* listP, int index);

void appendStateList(stateListeElem* listP, State* val);

void destroyStateList(stateListeElem* myStateList);
