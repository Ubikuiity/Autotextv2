#include "./headers/wordPatterns.h"

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

destroyWordPatterns(wordPatterns* patterns)
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
