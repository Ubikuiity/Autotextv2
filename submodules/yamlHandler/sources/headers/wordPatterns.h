#pragma once

#include "../../utils/headers/dynStrArray.h"

typedef struct wordPatterns wordPatterns;
struct wordPatterns
{
    strListeElem* words;  // Words that we are seeking in user inputs
    strListeElem* replacements;  // patterns that will replace the word
};

void displayWordPatterns(wordPatterns* patterns);
destroyWordPatterns(wordPatterns* patterns);
