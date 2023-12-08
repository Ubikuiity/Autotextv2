#include "./headers/dynStrArray.h"

// Creates the string list with first element
strListeElem* createStrListWithFirstElem(char* val)
{
    strListeElem* firstElem = malloc(sizeof(strListeElem));
    firstElem->value = val;
    firstElem->next = NULL;

    return firstElem;
}

// Creates from array of pointers. Use 'sizeof(myWords) / sizeof(myWords[0])' to get size
strListeElem* createStrListFromWords(char* words[], int sizeOfArray)
{
    strListeElem* firstElem = malloc(sizeof(strListeElem));
    firstElem->value = words[0];

    strListeElem* currentElem = firstElem;
    for(int i=1; i<sizeOfArray; i++)
    {
        strListeElem* nextElem = malloc(sizeof(strListeElem));
        currentElem->next = nextElem;

        nextElem->value = words[i];
        currentElem = nextElem;
    }
    currentElem->next = NULL;

    return firstElem;
}

char* getStrListeValue(strListeElem* listP, int index)
{
    if (listP == NULL)
    {
        // printf("String list is empty, returning NULL\n");
        return NULL;
    }
    for (int i=0; i<index; i++)
    {
        if (listP->next == NULL)
        {
            // printf("Coudn't get value, returning NULL : index out of range\n");
            return NULL;
        }
        else
        {
            listP = listP->next;
        }
    }
    return listP->value;
}

void destroyStrList(strListeElem* myStrList)
{
    strListeElem* nextElem = myStrList;

    while(nextElem != NULL)  // myStrList will take the value of the "previous" element
    {
        nextElem = nextElem->next;
        free(myStrList);
        myStrList = nextElem;
    }

    return;
}

void appendStrList(strListeElem* listP, char* val)
{
    while(listP->next != NULL)
    {
        listP = listP->next;
    }
    // Now listP should be the last value of the list

    strListeElem* newElem = malloc(sizeof(strListeElem));
    newElem->value = val;
    newElem->next = NULL;

    listP->next = newElem;  // set the link
}

// Calls callback on all values of list with parameters (index, string)
void itterateOverStrList(strListeElem* listP, void (*callback)(int, char*))
{
    int cmp = 0;
    while(listP->next != NULL)  // listP will take the value of the "previous" element
    {
        callback(cmp, listP->value);
        listP = listP->next;
        cmp++;
    }
    callback(cmp, listP->value);
}

char* popStrList(strListeElem* listP)
{
    strListeElem* previousElem = NULL;
    while(listP->next != NULL)
    {
        previousElem = listP;
        listP = listP->next;
    }
    // Now listP should be the last value of the list and previousElem the previous one

    char* returnValue = listP->value;
    if (previousElem != NULL)  // Set previous Elem next to NULL if the previous element exists (length > 1)
    {
        previousElem->next = NULL;
    }
    free(listP);

    return returnValue;
}

char* popIndexStrList(strListeElem** pointerToListP, int index)
{
    strListeElem* listP = *pointerToListP;
    strListeElem* previousElem = NULL;
    for (int i=0; i<index; i++)
    {
        if (listP->next == NULL)
        {
            // printf("Coudn't pop value, returning NULL : index out of range\n");
            return NULL;
        }
        else
        {
            previousElem = listP;
            listP = listP->next;
        }
    }
    // ListP should be at desired index, we will remove that element from the list

    char* returnValue = listP->value;
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

int lengthStrList(strListeElem* listP)
{
    int cmp = 1;
    while(listP->next != NULL)
    {
        listP = listP->next;
        cmp++;
    }
    return cmp;
}

// Get index of the given value. Returns -1 if no values matched the given string
int getIndexOfStrValue(strListeElem* listP, char* value)
{
    int cmp = 0;
    while(strcmp(listP->value, value) != 0)
    {
        if (listP->next == NULL) // If we hit the end of the list
        {
            return -1;
        }
        else
        {
            cmp++;
            listP = listP->next;
        }
    }
    return cmp;
}

// Creates a 'deep' copy of the given strList.
// The string still doesn't contains the strings but the pointers !
strListeElem* memCopyStrList(strListeElem* listP)
{
    strListeElem* newList = createStrListWithFirstElem(listP->value);
    listP = listP->next;
    while(listP != NULL)
    {
        appendStrList(newList, listP->value);
        listP = listP->next;
    }
    return newList;
}
