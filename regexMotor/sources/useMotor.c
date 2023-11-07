#include "./headers/useMotor.h"

#include <stdio.h>

void displayStateAndNext(State* state, int offSet);
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
    displayStateAndNext(firstState, 0);
}

void displayStateAndNext(State* state, int offSet)
{
    char buffer[10];  // max Id of state must be <1 000
    sprintf(buffer, "---%d:%c ", state->Id, state->condition);  // state->Id string size is variable (1->1, 10->2, 100->3)
    printf(buffer);
    int stateDisplaySize = customStrLen(buffer);

    stateListeElem* thisOuts= state->outs;

    for (int i=0; i<lengthStateList(thisOuts); i++)
    {
        if (i >= 1)  // if i = 0, we keep going on the same line, else we skip a line and add correct spacing to create a "branch"
        {
            printf("\n%*s┕", offSet + stateDisplaySize - 1, "");  // Add correct spacing at the beginning of next line
        }
        displayStateAndNext(getStateListeValue(thisOuts, i), offSet + stateDisplaySize);  // Care, the hardcoded 6 doesn't work if Id of state > 10
    }
}

int customStrLen(char str[])
{
    int cmp = 0;
    while(str[cmp] != '\0')
    {
        cmp++;
    }
    return cmp;
}