#pragma once

#include "../../../utils/headers/dynStrArray.h"
#include <stdlib.h>

// These are the string used in file to be able to parse it
#define PATTERN_HEADER "\n-Pattern:\n"
#define WORD_HEADER "w0rD"
#define REPLACE_HEADER "r3Pl4c3"

typedef struct wordPatterns wordPatterns;
struct wordPatterns
{
    strListeElem* words;  // Words that we are seeking in user inputs
    strListeElem* replacements;  // patterns that will replace the word
};

wordPatterns* createWordPatternsFromFirstPair(const char* word, const char* replacer);

void appendPattern(wordPatterns* patterns, const char* word, const char* replacer);
// void removePattern(wordPatterns* pattern, int index);
// void changePatternWord(wordPatterns* pattern, int index, char* word);
// void changePatternReplacer(wordPatterns* pattern, int index, char* replacer);

void displayWordPatterns(wordPatterns* patterns);
void destroyWordPatterns(wordPatterns* patterns);
char* getReplacerFromWord(wordPatterns* patterns, char* word);
