#include "./headers/yamlWriter.h"

// This functions takes a wordPatterns structure as input and writes a file that can be read to recreate the wordPatterns structure
void writeFileFromWordPatterns(const char* pathOfFile, wordPatterns* patterns)
{
    FILE* file = NULL;
    file = fopen(pathOfFile, "w");
    if (file == NULL)
    {
        printf("Something unexpected happend when opening");
        return;
    }

    int nbPatterns = lengthStrList(patterns->words);
    // We itterate over each pattern and write it to the file
    for(int iPattern=0; iPattern<nbPatterns; iPattern++)
    {
        fputs(PATTERN_HEADER, file);  // Add main header
        fprintf(file, "\t%s:%s:%s\n", WORD_HEADER, getStrListeValue(patterns->words, iPattern), WORD_HEADER);  // Add word
        fprintf(file, "\t%s:%s:%s\n", REPLACE_HEADER, getStrListeValue(patterns->replacements, iPattern), REPLACE_HEADER);  // Add replacer
    }

    // Once we've done each pattern, we close the file
    fclose(file);
}