#pragma once

#include <stdio.h>
#include "wordPatterns.h"

// Public functions

wordPatterns* getWordPatternsFromFile(const char* filePath);

// Private functions

char* fileToString(const char* pathOfFile);
wordPatterns* findWordsAndReplacements(char* buffer);
void findWordAndReplacer(wordPatterns* Patterns, char* buffer, int* firstRun);
char* findLine(char** bufferPtr, char* lineName);
int findLineLength(char* buffer, char* lineName);
int customStrCmp(char* str1, char* str2);
