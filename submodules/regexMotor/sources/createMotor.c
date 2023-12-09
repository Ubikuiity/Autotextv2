#include "./headers/createMotor.h"

int stateCounter = 0;

typedef struct stringWithIndex stringWithIndex;
struct stringWithIndex
{
    char previousChars[MAXINPUTSTRINGSIZE + 1];
    int indexPreviousChars;  // Index of the '\0' in the list
};

void createNextBranches(State* callingState, strListeElem* callingWords, reMotor* motor, stringWithIndex* previousChars, int reccursive);
void appendToString(stringWithIndex* strStruct, char c);
void deepCopyStringWithIndex(stringWithIndex* Destination, stringWithIndex* Source);

// The regex motor we use has the form of a state machine
// Returns initial state and creates all the states and branches
reMotor* createMotor(strListeElem* words)
{
    stateCounter = 0;
    strListeElem* dynWords = memCopyStrList(words);  // We copy the list since we will destroy it we create the motor
    reMotor* regexMotor = malloc(sizeof(reMotor));

    State* firstState = createState(stateCounter, NULL, '\0');
    stateCounter++;
    regexMotor->StateList = createStateListWithFirstElem(firstState);

    // Here we initialize the values of FinalStrings and FinalStringIndexes of motor with false values that we will remove later
    // We need to have one value to initialize the list, this is why we  create false values
    regexMotor->FinalStrings = createStrListWithFirstElem("a");
    regexMotor->FinalStates = createIntListWithFirstElem(0);

    stringWithIndex* initialCharacters = malloc(sizeof(stringWithIndex));
    initialCharacters->indexPreviousChars = 0;
    initialCharacters->previousChars[0] = '\0';
    createNextBranches(firstState, dynWords, regexMotor, initialCharacters, 1);

    popIndexStrList(&(regexMotor->FinalStrings), 0);  // Remove the false values we created to initialize
    popIndexIntList(&(regexMotor->FinalStates), 0);  // Remove the false values we created to initialize
    regexMotor->Actives = createIntListWithFirstElem(0);  // Set initial state to active

    int lengthOfLongestWord = getMaxLengthOfStringInList(words);
    // We cannot put actives in the activesHistory yet since it will create a duplicate when we run motor for the first time
    regexMotor->ActivesHistory = createIntListStackWithFirstElem(createIntListWithFirstElem(0), lengthOfLongestWord + 1);

    return regexMotor;
}

// reccursive must be 1 if we want the function to be reccursive.
// This function is a reccursive function that creates the branches exiting the calling state.
void createNextBranches(State* callingState, strListeElem* callingWords, reMotor* motor, stringWithIndex* previousChars, int reccursive)
{
    int firstRound = 1;  // Used to initialize outs of callingState

    while(callingWords != NULL)  // Calling words will be destroyed throught the itterations, we stop when no words are left
    {
        // This part creates the new state and append it to the previous State's outputs
        char* firstWord = popIndexStrList(&callingWords, 0);
        State* NewState = createState(stateCounter, NULL, firstWord[0]);  // Creating the state
        stateCounter++;

        appendStateList(motor->StateList, NewState);

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
            appendIntList(motor->FinalStates, NewState->Id);

            // Recover final word
            char* finalWord = malloc(sizeof(char[MAXINPUTSTRINGSIZE + 1])); // We need to manually allocate here since variable is destroyed when function ends otherwise
            strcpy(finalWord, previousChars->previousChars);
            int lenWord = strlen(finalWord);
            finalWord[lenWord] = firstWord[0];
            finalWord[lenWord + 1] = '\0';
            appendStrList(motor->FinalStrings, finalWord);
            continue;  // Go to next word, this state has no outputs since it is a final state
        }
        // Else we deal with the newly created state to create its ouputs

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
            else
            {
                i++;  // We only iterate if we didn't pop the list, otherwise the popping is in itself of itteration since we remove an element
            }
            word = getStrListeValue(callingWords, i);
        }
        if (reccursive == 1)
        {
            // Prepare the new previous characters
            stringWithIndex* nextPreviousChars = malloc(sizeof(stringWithIndex));
            deepCopyStringWithIndex(nextPreviousChars, previousChars);
            appendToString(nextPreviousChars, firstWord[0]);
            createNextBranches(NewState, nextWords, motor, nextPreviousChars, reccursive);
        }
    }
    free(previousChars);  // Free the calling previous chars
}

void appendToString(stringWithIndex* strStruct, char c)
{
    if (strStruct->indexPreviousChars >= MAXINPUTSTRINGSIZE - 1)  // Skip if we have an overflow
    {
        return;
    }
    else
    {
        strStruct->previousChars[strStruct->indexPreviousChars] = c;
        strStruct->previousChars[strStruct->indexPreviousChars + 1] = '\0'; 
    }
    strStruct->indexPreviousChars++;
}

void deepCopyStringWithIndex(stringWithIndex* Destination, stringWithIndex* Source)
{
    // Handle the deep copy of the string
    int cmp = -1;  // We increment at the begining of the loop, so we start at -1
    while(Source->previousChars[cmp] != '\0')
    {
        cmp ++;
        Destination->previousChars[cmp] = Source->previousChars[cmp];
    }
    Destination->indexPreviousChars = Source->indexPreviousChars;
    return;
}

// Used in destructor function to destroy final strings
void freeString(int i, char* str)
{
    free(str);
}

void destroyMotor(reMotor* motor)
{
    destroyStateList(motor->StateList);
    destroyIntList(motor->Actives);
    destroyIntList(motor->FinalStates);

    // We have to manually destroy each final strings that has been allocated when creating the motor
    itterateOverStrList(motor->FinalStrings, freeString);
    destroyStrList(motor->FinalStrings);
    destroyIntListStack(motor->ActivesHistory);
    free(motor);
}
