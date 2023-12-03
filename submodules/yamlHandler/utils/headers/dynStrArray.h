#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// This structure is a pointer list, it doesn't contain the strings in itself
typedef struct strListeElem strListeElem;
struct strListeElem
{
    char* value;  // Pointer to the string stored
    strListeElem* next;
};

strListeElem* createStrListWithFirstElem(char* val);
strListeElem* createStrListFromWords(char* words[], int sizeOfArray);

char* getStrListeValue(strListeElem* list, int place);

void appendStrList(strListeElem* listP, char* val);
char* popStrList(strListeElem* listP);
char* popIndexStrList(strListeElem** pointerToListP, int index);

int lengthStrList(strListeElem* listP);
void itterateOverStrList(strListeElem* listP, void (*callback)(int, char*));
void destroyStrList(strListeElem* myIntList);
int getIndexOfStrValue(strListeElem* listP, char* value);
