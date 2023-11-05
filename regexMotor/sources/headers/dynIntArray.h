#pragma once

#include <stdlib.h>
#include <stdio.h>

typedef struct intListeElem intListeElem;
struct intListeElem
{
    int value;  // int value
    intListeElem* next;
};

intListeElem* createIntListWithFirstElem(int val);

void appendIntList(intListeElem* listP, int val);

int* getIntListeValue(intListeElem* listP, int index);
void itterateOverIntList(intListeElem* listP, void (*callback)(int index, int value));

void destroyIntList(intListeElem* myIntList);

