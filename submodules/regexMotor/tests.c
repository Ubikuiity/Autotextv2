#include "sources/headers/useMotor.h"

#include <stdio.h>
#include <stdlib.h>

void printStrValue(int index, char* word);
void testDynStrArrays();
reMotor* testMotorCreation();
void testMotorUse(reMotor* motor, char* strToTest);
void testColoredPrints();

int main ()
{
    // testDynStrArrays();
    reMotor* myMotor = testMotorCreation();
    // testMotorUse(myMotor, "slicat");
    // testColoredPrints();

    destroyMotor(myMotor);

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

    itterateOverStrList(myList, printStrValue);

    int indexRemove = 2;
    printf("\nRemoving element nb : %i --- %s\n\n", indexRemove, popIndexStrList(&myList, indexRemove));

    itterateOverStrList(myList, printStrValue);

    printf("\nRemoving last element : %s\n\n", popStrList(myList));

    itterateOverStrList(myList, printStrValue);

    printf("--- Testing construction of dynamic array from static array ---\n\n");

    char* myWords[] = {"Hi", "Hello", "This2", "erf...", "X"};
    int sizeOfMyWords = sizeof(myWords) / sizeof(myWords[0]);

    strListeElem* my2List = createStrListFromWords(myWords, sizeOfMyWords);

    itterateOverStrList(my2List, printStrValue);

    printf("\nRemoving last element : %s\n", popStrList(my2List));
    printf("Removing first element : %s\n", popIndexStrList(&my2List, 0));
    printf("Removing third element : %s\n\n", popIndexStrList(&my2List, 2));

    itterateOverStrList(my2List, printStrValue);
}

void printStrValue(int index, char* word)
{
    printf("index : %i --- %s\n", index, word);
}

void printIntValue(int index, int val)
{
    printf("%i ", val);
}

reMotor* testMotorCreation()
{
    char* myWords[] = {"hello", "cat", "slay", "slice", "slicat", "snake", "hi"};
    strListeElem* myWordsList = createStrListFromWords(myWords, sizeof(myWords) / sizeof(myWords[0]));
    reMotor* myMotor = createMotor(myWordsList);

    int numberOfStates = lengthStateList(myMotor->StateList);
    printf("\nMachine has %d states\n\n", numberOfStates);
    printf("Final states are states : ");
    itterateOverIntList(myMotor->FinalStates, printIntValue);
    printf("\nFinal strings are : \n");
    itterateOverStrList(myMotor->FinalStrings, printStrValue);
    printf("\n");

    // for (int i=0; i<numberOfStates; i++)
    // {
    //     State* theState = getStateListeValue(myMotor->StateList, i);
    //     displayState(theState);
    //     printf("\n\n");
    // }

    plotMotor(myMotor);

    return myMotor;
}

void testMotorUse(reMotor* motor, char* strToTest)
{
    int lenOfString = strlen(strToTest);
    for (int i=0; i<lenOfString; i++)
    {
        printf("\n ----- Iteration %d ----- Character : %c\n\n", i + 1, strToTest[i]);
        nextStep(motor, strToTest[i]);
        plotMotor(motor);
    }
    return;
}

void testColoredPrints()
{
    printf("\n\n --- Testing the color changes of prints ---\n\n");
    for(int i=0; i<8; i++)
    {
        changePrintColor(i);
        printf("%d", i);
    }
    changePrintColor(-1);
    printf("\nBack to default color.\n\n");
}
