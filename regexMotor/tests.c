#include "sources/headers/dynStrArray.h"
#include "sources/headers/createMotor.h"
#include "sources/headers/useMotor.h"

#include <stdio.h>

void printValue(int index, char* word);
void testDynStrArrays();
void testMotorCreation();

int main ()
{
    // testDynStrArrays();
    testMotorCreation();

    return 0;
}

void testDynStrArrays()
{
    printf("--- Testing dynStrArrays module ---\n\n");
    char str1[] = "String1";
    char str2[] = "Stri2";
    char str3[] = "St3";

    strListeElem* myList = createStrListWithFirstElem("Hello");
    appendStrList(myList, str1);
    appendStrList(myList, str2);
    appendStrList(myList, str3);
    appendStrList(myList, "WrittenInFunction");

    itterateOverStrList(myList, printValue);

    int indexRemove = 2;
    printf("\nRemoving element nb : %i --- %s\n\n", indexRemove, popIndexStrList(&myList, indexRemove));

    itterateOverStrList(myList, printValue);

    printf("\nRemoving last element : %s\n\n", popStrList(myList));

    itterateOverStrList(myList, printValue);

    printf("--- Testing construction of dynamic array from static array ---\n\n");

    char* myWords[] = {"Hi", "Hello", "This2", "erf...", "X"};
    int sizeOfMyWords = sizeof(myWords) / sizeof(myWords[0]);

    strListeElem* my2List = createStrListFromWords(myWords, sizeOfMyWords);

    itterateOverStrList(my2List, printValue);

    printf("\nRemoving last element : %s\n", popStrList(my2List));
    printf("Removing first element : %s\n", popIndexStrList(&my2List, 0));
    printf("Removing third element : %s\n\n", popIndexStrList(&my2List, 2));

    itterateOverStrList(my2List, printValue);
}

void printValue(int index, char* word)
{
    printf("index : %i --- %s\n", index, word);
}

void testMotorCreation()
{
    char* myWords[] = {"Hi", "Hello", "Cat", "Slay", "Slice", "Snake"};
    reMotor* myMotor = createMotor(myWords, sizeof(myWords) / sizeof(myWords[0]));

    int numberOfStates = lengthStateList(myMotor->StateList);
    printf("\nMachine has %d states\n\n", numberOfStates);

    for (int i=0; i<numberOfStates; i++)
    {
        State* theState = getStateListeValue(myMotor->StateList, i);
        displayState(theState);
        printf("\n\n");
    }

    plotMotor(myMotor);
}