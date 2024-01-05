#include "./headers/wordPatterns.h"

wordPatterns* createWordPatternsFromFirstPair(const char* word, const char* replacer)
{
    wordPatterns* newPatterns = (wordPatterns*) malloc(sizeof(wordPatterns));

    newPatterns->words = createStrListWithFirstElem(strdup(word));
    newPatterns->replacements = createStrListWithFirstElem(strdup(replacer));

    return newPatterns;
}

// Used a debug function to print all the patterns
void displayWordPatterns(wordPatterns* patterns)
{
    printf("\nExploring word patterns...\nword : %i\nreplacers : %i\n\n", lengthStrList(patterns->words),lengthStrList(patterns->replacements));
    for(int i=0; i<lengthStrList(patterns->words); i++)
    {
        printf("\nPlotting pattern %i\n", i);
        printf("Word:\n%s\n", getStrListeValue(patterns->words, i));
        printf("Replacer:\n%s\n", getStrListeValue(patterns->replacements, i));
    }
}

// Destroy and free the memory of the patterns and the structure containing it
void destroyWordPatterns(wordPatterns* patterns)
{
    for(int i=0; i<lengthStrList(patterns->words); i++)
    {
        free(getStrListeValue(patterns->words, i));
        free(getStrListeValue(patterns->replacements, i));
    }
    destroyStrList(patterns->words);
    destroyStrList(patterns->replacements);
    free(patterns);

}

// Appends a new pattern at the end of the list
// The strings will be given a new place in memory, so the strings given as parameters can be freed after this has been called
void appendPattern(wordPatterns* patterns, const char* word, const char* replacer)
{
    appendStrList(patterns->words, strdup(word));  // strdup does the malloc
    appendStrList(patterns->replacements, strdup(replacer));
}

// // Removes the pattern at given index, or nothing if index is out of range
// void removePattern(wordPatterns* pattern, int index)
// {
//     if (lengthStrList(pattern->words) <= index)  // If we are trying to remove out of range index
//     {
//         return;
//     }
//     popIndexStrList(&(pattern->words), index);
//     popIndexStrList(&(pattern->replacements), index);
// }

// // Replace the pattern word at index i
// void changePatternWord(wordPatterns* pattern, int index, char* word)
// {
//     if (lengthStrList(pattern->words) <= index)  // If we are trying to change out of range index
//     {
//         return;
//     }
//     free(getStrListeValue(pattern->words, index));  // We free the stored string
//     changeIndexValueStrList(pattern->words, index, strdup(word));  // This allocates memory for the new string
// }

// // Replace the replacer string at index i
// void changePatternReplacer(wordPatterns* pattern, int index, char* replacer)
// {
//     if (lengthStrList(pattern->words) <= index)  // If we are trying to change out of range index
//     {
//         return;
//     }
//     free(getStrListeValue(pattern->replacements, index));  // We free the stored string
//     changeIndexValueStrList(pattern->words, index, strdup(replacer));  // Allocate memory for the new string
// }

// Gets replacer for the given word. NULL if no words were found
char* getReplacerFromWord(wordPatterns* patterns, char* word)
{
    int indexOfWord = getIndexOfStrValue(patterns->words, word);
    if (indexOfWord != -1)
    {
        return getStrListeValue(patterns->replacements, indexOfWord);
    }
    else
    {
        return NULL;
    }
}
