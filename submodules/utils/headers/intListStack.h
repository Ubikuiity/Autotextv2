#pragma once

#include "dynIntArray.h"

// This header describes an enhanced version of an intListeElem stack (a stack containing intListeElems)
// This structure will automatically destroy the last list if the maximum size is reached and we try to add a new element

typedef struct intListStackElem intListStackElem;
struct intListStackElem
{
    intListeElem* value;  // list being pointed by the stack element
    intListStackElem* previous;
    intListStackElem* next;
};

typedef struct intListStack intListStack;
struct intListStack
{
    intListStackElem* first;
    intListStackElem* last;
    int maxSize;  // Determined maximum size of the stack
    int currentSize;
};

// Public functions 

intListStack* createIntListStackWithFirstElem(intListeElem* elem, int maxSize);
void stackIntListStack(intListStack* stack, intListeElem* elem);
intListeElem* unstackIntListStack(intListStack* stack);
void destroyIntListStack(intListStack* stack);
void printIntListStack(intListStack* stack);

// Private functions

void removeLastElem(intListStack* stack);
