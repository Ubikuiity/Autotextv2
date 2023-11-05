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
            printf("Coudn't get value, returning 0 : index out of range\n");
            return 0;
        }
        else
        {
            listP = listP->next;
        }
    }
    return &(listP->value);  // Return pointer in case we need to modify value
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
