#include "./headers/dynIntArray.h"

intListeElem* createIntListWithFirstElem(int val)
{
    intListeElem* firstElem = malloc(sizeof(intListeElem));
    firstElem->value = val;
    firstElem->next = NULL;

    return firstElem;
}

void appendIntList(intListeElem* listP, int val)
{
    while(listP->next != NULL)
    {
        listP = listP->next;
    }
    // Now listP should be the last value of the list

    intListeElem* newElem = malloc(sizeof(intListeElem));
    newElem->value = val;
    newElem->next = NULL;

    listP->next = newElem;  // set the link
}

int* getIntListeValue(intListeElem* listP, int index)
{
    for (int i=0; i<index; i++)
    {
        if (listP->next == NULL)
        {
            // printf("Coudn't get value, returning 0 : index out of range\n");
            return 0;
        }
        else
        {
            listP = listP->next;
        }
    }
    return &(listP->value);  // Return pointer in case we need to modify value
}

int popIndexIntList(intListeElem** pointerToListP, int index)
{
    intListeElem* listP = *pointerToListP;
    intListeElem* previousElem = NULL;
    for (int i=0; i<index; i++)
    {
        if (listP->next == NULL)
        {
            // printf("Coudn't pop value, returning -1 : index out of range\n");
            return -1;
        }
        else
        {
            previousElem = listP;
            listP = listP->next;
        }
    }
    // ListP should be at desired index, we will remove that element from the list

    int returnValue = listP->value;
    if (previousElem == NULL)  // If we never went in the loop = we are removing first element
    {
        *pointerToListP = listP->next;  // We change the pointer so it points to the next element
    }
    else
    {
        previousElem->next = listP->next;  // We change previous element so it points to the next + 1
    }
    free(listP);
    return returnValue;
}

void itterateOverIntList(intListeElem* listP, void (*callback)(int index, int value))
{
    int cmp = 0;
    while(listP->next != NULL)
    {
        callback(cmp, listP->value);
        listP = listP->next;
        cmp++;
    }
    callback(cmp, listP->value);
}

// Returns 1 if value is int list, 0 otherwise
int isInList(intListeElem* listP, int value)
{
    while(listP->next != NULL)
    {
        if(listP->value == value)
        {
            return 1;
        }
        listP = listP->next;
    }
    if(listP->value == value)  // Check last value of list
    {
        return 1;
    }
    return 0;
}

int getIndexOfValue(intListeElem* listP, int value)
{
    int cmp = 0;
    while(listP->next != NULL)
    {
        if(listP->value == value)
        {
            return cmp;
        }
        listP = listP->next;
        cmp++;
    }
    if(listP->value == value)  // Check last value of list
    {
        return cmp;
    }
    return -1;
}

int lengthIntList(intListeElem* listP)
{
    if (listP == NULL)  // if list is empty
    {
        return 0;
    }
    int cmp = 1;
    while(listP->next != NULL)
    {
        listP = listP->next;
        cmp++;
    }
    return cmp;
}

void destroyIntList(intListeElem* myIntList)
{
    intListeElem* nextElem = myIntList;

    while(nextElem != NULL)  // myIntList will take the value of the "previous" element
    {
        nextElem = nextElem->next;
        free(myIntList);
        myIntList = nextElem;
    }

    return;
}
