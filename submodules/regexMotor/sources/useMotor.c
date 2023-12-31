#include "./headers/useMotor.h"

#include <stdio.h>

void displayStateAndNext(State* state, reMotor* motor, int offSet);

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
    displayStateAndNext(firstState, motor, 0);
    printf("\n");  // Skip a line before returning
}

void displayStateAndNext(State* state, reMotor* motor, int offSet)
{
    char buffer[10];  // max Id of state must be <1 000
    if (isInList(motor->FinalStates, state->Id))
    {
        changePrintColor(1);  // If state is final we print in Red
    }
    if (isInList(motor->Actives, state->Id))
    {
        changePrintColor(2);  // If state is active, we print in Green
    }
    sprintf(buffer, "---%d:%c ", state->Id, state->condition);  // state->Id string size is variable (1->1, 10->2, 100->3)
    printf(buffer);
    
    if (isInList(motor->FinalStates, state->Id))  // If state is final, we print the final string leading to this state
    {
        char* stringFinal = getStrListeValue(motor->FinalStrings, getIndexOfValue(motor->FinalStates, state->Id));
        printf("-> %s", stringFinal);
    }

    changePrintColor(-1);  // Set color back to default

    int stateDisplaySize = strlen(buffer);

    stateListeElem* thisOuts= state->outs;

    for (int i=0; i<lengthStateList(thisOuts); i++)
    {
        if (i >= 1)  // if i = 0, we keep going on the same line, else we skip a line and add correct spacing to create a "branch"
        {
            printf("\n%*s", offSet + stateDisplaySize, "");  // Add correct spacing at the beginning of next line
        }
        displayStateAndNext(getStateListeValue(thisOuts, i), motor, offSet + stateDisplaySize);  // Care, the hardcoded 6 doesn't work if Id of state > 10
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
    
    // Prepare for next run if we didn't hit a final state
    stackIntListStack(motor->ActivesHistory, motor->Actives);  // We add the former active states to history
    motor->Actives = nextActives;
    return;
}

void undoLastStep(reMotor* motor)
{
    if (motor->ActivesHistory->currentSize > 1)
    {
        destroyIntList(motor->Actives);  // Destroy current actives
        motor->Actives = unstackIntListStack(motor->ActivesHistory);  // Recover older states
    }
    else  // If we've reached the end of history, we keep resetting to initial state
    {
        destroyIntList(motor->Actives);
        motor->Actives = createIntListWithFirstElem(0);
    }
}

// Checks if we hit any final state, returns the index of the final state if we did (this index will never be 0), 0 otherwise
int checkAndGetFinals(reMotor* motor)
{
    for (int i=0; i<lengthIntList(motor->Actives); i++)
    {
        int activeStateIndex = *getIntListeValue(motor->Actives, i);
        State* activeState = getStateListeValue(motor->StateList, activeStateIndex);
        if (activeState->outs == NULL)  // State is final if there is no outs
        {
            return activeStateIndex;
        }
    }
    return 0;
}

// Reset the active states for a new run
void restartMotorForNextRun(reMotor* motor)
{
    destroyIntList(motor->Actives);
    motor->Actives = createIntListWithFirstElem(0);

    // Reset of active states history
    int maxHistorySize = motor->ActivesHistory->maxSize;
    destroyIntListStack(motor->ActivesHistory);  // Delete history
    // We cannot put actives in the activesHistory yet since it will create a duplicate when we run motor for the first time
    motor->ActivesHistory = createIntListStackWithFirstElem(createIntListWithFirstElem(0), maxHistorySize);
}