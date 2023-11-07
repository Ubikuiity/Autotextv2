#include "./headers/dynStateArray.h"

State* createState(int Id, stateListeElem* outs, char condition)
{
    State* initialState = malloc(sizeof(State));
    initialState->Id = Id;
    initialState->outs = outs;
    initialState->condition = condition;
    
    return initialState;
}

// Creates the string list with first element
stateListeElem* createStateListWithFirstElem(State* val)
{
    stateListeElem* firstElem = malloc(sizeof(stateListeElem));
    firstElem->value = val;
    firstElem->next = NULL;

    return firstElem;
}

State* getStateListeValue(stateListeElem* listP, int index)
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
    return listP->value;
}

void destroyStateList(stateListeElem* myStateList)
{
    stateListeElem* nextElem = myStateList;

    while(nextElem != NULL)  // myStateList will take the value of the "previous" element
    {
        nextElem = nextElem->next;
        free(myStateList);
        myStateList = nextElem;
    }

    return;
}

void appendStateList(stateListeElem* listP, State* val)
{
    while(listP->next != NULL)
    {
        listP = listP->next;
    }
    // Now listP should be the last value of the list

    stateListeElem* newElem = malloc(sizeof(stateListeElem));
    newElem->value = val;
    newElem->next = NULL;

    listP->next = newElem;  // set the link
}

int lengthStateList(stateListeElem* listP)
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