#pragma once

#include "../../utils/headers/dynStrArray.h"

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

void appendPattern(wordPatterns* patterns, char* word, char* replacer);
void displayWordPatterns(wordPatterns* patterns);
void destroyWordPatterns(wordPatterns* patterns);
