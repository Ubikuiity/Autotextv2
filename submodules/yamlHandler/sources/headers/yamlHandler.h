#pragma once

#include <stdio.h>
#include "wordPatterns.h"

#define PATTERN_HEADER "\n-Pattern:\n"
#define WORD_HEADER "w0rD"
#define REPLACE_HEADER "r3Pl4c3"

// Public functions

char* fileToString(char* pathOfFile);
wordPatterns* findWordsAndReplacements(char* buffer);

// Private functions

void findWordsAndReplacers(wordPatterns* Patterns, char* buffer, int* firstRun);
char* findLine(char** bufferPtr, char* lineName);
int findLineLength(char* buffer, char* lineName);
int customStrCmp(char* str1, char* str2);
