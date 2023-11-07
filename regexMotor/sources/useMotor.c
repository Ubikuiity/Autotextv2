#include "./headers/useMotor.h"

#include <stdio.h>

void displayStateAndNext(State* state, intListeElem* activeStates, int offSet);
int customStrLen(char str[]);

void displayState(State* state)
{
    printf("State %d :\n   Outs : ", state->Id);
    stateListeElem* thisOuts = state->outs;
    for(int i=0; i<lengthStateList(thisOuts); i++)
    {
        printf("%d, ", getStateListeValue(thisOuts, i)->Id);
    }
    printf("\n   Condition : %c", state->condition);
}

void plotMotor(reMotor* motor)
{
    State* firstState = getStateListeValue(motor->StateList, 0);
    displayStateAndNext(firstState, motor->Actives, 0);
    printf("\n");  // Skip a line before returning
}

void displayStateAndNext(State* state, intListeElem* activeStates, int offSet)
{
    char buffer[10];  // max Id of state must be <1 000
    if (isInList(activeStates, state->Id))
    {
        changePrintColor(2);
    }
    sprintf(buffer, "---%d:%c ", state->Id, state->condition);  // state->Id string size is variable (1->1, 10->2, 100->3)
    printf(buffer);
    if (isInList(activeStates, state->Id))
    {
        changePrintColor(-1);  // Set color back to default
    }
    int stateDisplaySize = customStrLen(buffer);

    stateListeElem* thisOuts= state->outs;

    for (int i=0; i<lengthStateList(thisOuts); i++)
    {
        if (i >= 1)  // if i = 0, we keep going on the same line, else we skip a line and add correct spacing to create a "branch"
        {
            printf("\n%*s┕", offSet + stateDisplaySize - 1, "");  // Add correct spacing at the beginning of next line
        }
        displayStateAndNext(getStateListeValue(thisOuts, i), activeStates, offSet + stateDisplaySize);  // Care, the hardcoded 6 doesn't work if Id of state > 10
    }
}

void nextStep(reMotor* motor, char inputChar)
{
    intListeElem* nextActives = createIntListWithFirstElem(0);
    for (int activeIndex=0; activeIndex<lengthIntList(motor->Actives); activeIndex++)  // For each active state
    {
        State* currentState = getStateListeValue(motor->StateList, *getIntListeValue(motor->Actives, activeIndex));
        for (int outIndex=0; outIndex<lengthStateList(currentState->outs); outIndex++)  // We look for all possible out states
        {
            State* outState = getStateListeValue(currentState->outs, outIndex);
            if (inputChar == outState->condition)  // If the condition to enter out state is OK
            {
                if (!isInList(nextActives, outState->Id))  // we add the state to next actives if it not already in it
                {
                    appendIntList(nextActives, outState->Id);
                }
            }
        }
    }
    // We need to check if some states in next actives are final states
    destroyIntList(motor->Actives);  // We remove old active list
    motor->Actives = nextActives;
    return;
}

// Utility functions

int customStrLen(char str[])
{
    int cmp = 0;
    while(str[cmp] != '\0')
    {
        cmp++;
    }
    return cmp;
}