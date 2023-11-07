#include "./headers/createMotor.h"

int stateCounter = 0;

void createNextBranches(State* callingState, strListeElem* callingWords, stateListeElem* motorStates, int reccursive);

// The regex motor we use has the form of a state machine
// Returns initial state and creates all the states and branches
reMotor* createMotor(char* words[], int sizeWords)
{
    stateCounter = 0;
    strListeElem* dynWords = createStrListFromWords(words, sizeWords);
    reMotor* regexMotor = malloc(sizeof(reMotor));

    State* firstState = createState(stateCounter, NULL, '\0');
    stateCounter++;
    regexMotor->StateList = createStateListWithFirstElem(firstState);

    createNextBranches(firstState, dynWords, regexMotor->StateList, 1);

    return regexMotor;
}

// reccursive must be 1 if we want the function to be reccursive.
// This function is a reccursive function that creates the branches exiting the calling state.
void createNextBranches(State* callingState, strListeElem* callingWords, stateListeElem* motorStates, int reccursive)
{
    int firstRound = 1;  // Used to initialize outs of callingState

    while(callingWords != NULL)  // Calling words will be destroyed throught the itterations, we stop when no words are left
    {
        // This part creates the new state and append it to the previous State's outputs
        char* firstWord = popIndexStrList(&callingWords, 0);
        State* NewState = createState(stateCounter, NULL, firstWord[0]);  // Creating the state
        stateCounter++;

        appendStateList(motorStates, NewState);

        // Appending new state to callingState outputs
        if (firstRound == 1)
        {
            callingState->outs = createStateListWithFirstElem(NewState);
            firstRound = 0;
        }
        else
        {
            appendStateList(callingState->outs, NewState);  // Adding this state to the previous state outputs
        }
        if (firstWord[1] == '\0')  // If this is a single caracter, we create a final state
        {
            continue;  // Go to next word, this state has no outputs since it is a final state
        }
        // We now deal with the newly created state to create its ouputs

        strListeElem* nextWords = createStrListWithFirstElem(firstWord + 1);

        int i = 0;
        char* word = getStrListeValue(callingWords, i);
        while(word != NULL)
        {
            if (word[0] == firstWord[0])  // If both share the same first caracter
            {
                popIndexStrList(&callingWords, i);
                appendStrList(nextWords, word + 1);
            }
            i++;
            word = getStrListeValue(callingWords, i);
        }
        if (reccursive == 1)
        {
            createNextBranches(NewState, nextWords, motorStates, reccursive);
        }
    }
}
