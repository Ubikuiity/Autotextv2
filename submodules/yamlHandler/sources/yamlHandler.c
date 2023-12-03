#include "./headers/yamlHandler.h"

// Now functions that get the file and treat the text in it to find words
// ! We need to free the returned buffer after use !
char* fileToString(char* pathOfFile)
{
    FILE* file = NULL;
    file = fopen(pathOfFile, "r");
    if (file == NULL)
    {
        printf("Coudn't read file, unexpected error");
        return NULL;
    }
    fseek(file, 0, SEEK_END);  // Get to the end
    int length = ftell(file);  // Get the position of the end
    fseek(file, 0, SEEK_SET);  // Get back to beginning

    // We need to malloc inside of function since we don't know the size before calling the function
    char* buffer = malloc(sizeof(char) * (length + 1));
    if (buffer)
    {
        fread(buffer, 1, length, file);
    }
    buffer[length] = '\0';

    fclose(file);

    return buffer;
}

wordPatterns* findWordsAndReplacements(char* buffer)
{
    wordPatterns* Patterns = malloc(sizeof(wordPatterns));
    int firstRun = 1;

    int index = 0;

    while(1)
    {
        // printf("Value of string buffer :\n%s\n", buffer);
        while(customStrCmp(buffer + index, PATTERN_HEADER) != 0)  // Advance through the buffer till we find the pattern
        {
            if((buffer + index)[10] == '\0')  // Means we reached end of the file
            {
                // printf("Found end of file\n");
                char* result = malloc(sizeof(char) * (index + 1));
                strncpy(result, buffer, index);
                result[index] = '\0';
                
                findWordsAndReplacers(Patterns, result, &firstRun);

                free(result);  // We can free result because findWordsAndReplacers will alocate values and put them in Patterns
                
                return Patterns;
            }
            index ++;
        }

        // printf("Found the pattern with index %i !\n", index);
        // Once we found our pattern
        char* result = malloc(sizeof(char) * (index + 1));  // We get first part of slice in result
        strncpy(result, buffer, index);
        result[index] = '\0';

        findWordsAndReplacers(Patterns, result, &firstRun);

        free(result);  // We can free result because findWordsAndReplacers will alocate values and put them in Patterns

        buffer = buffer + index + strlen(PATTERN_HEADER);  // offsets buffer by the index + size of the header

        index = 0;
    }

    return Patterns;
}

// finds the words and replacers in the given buffer
void findWordsAndReplacers(wordPatterns* Patterns, char* buffer, int* firstRun)
{
    char* word = findLine(&buffer, WORD_HEADER);
    char* replace = findLine(&buffer, REPLACE_HEADER);

    if (*firstRun)
    {
        Patterns->words = createStrListWithFirstElem(word);
        Patterns->replacements = createStrListWithFirstElem(replace);
        *firstRun = 0;
    }
    else
    {
        appendStrList(Patterns->words, word);
        appendStrList(Patterns->replacements, replace);
    }
}

// Find and returns the content of line starting with "   X:" where X is specified as parameter
// This functions changes buffer to remove everything before the line with the pattern
// ! The returned string is allocated inside the function !
char* findLine(char** bufferPtr, char* lineName)
{
    char* buffer = *bufferPtr;
    int index = 0;
    while(buffer[index] != '\0')
    {
        if(buffer[index] == '\n' || index == 0)  // When we skip a line or at the beginning of the file, analyse the next line
        {
            index ++;
            while(buffer[index] == ' ' || buffer[index] == '\t')  // We skip tabs and spaces
            {
                index++;
            }
            if(customStrCmp(buffer + index, lineName) == 0)  // If we find the pattern, we return the pointer after the pattern
            {
                buffer = buffer + index + strlen(lineName) + 1;  // buffer is at the beginning of the string after the pattern, +1 for the ':'
                int len = findLineLength(buffer, lineName);  // Finds the length before we find ":X"
                char* resLine = malloc(sizeof(char) * (len + 1));
                strncpy(resLine, buffer, len);
                resLine[len] = '\0';

                *bufferPtr = buffer + len + strlen(lineName) + 1;  // We add one for the ':'
                return resLine;
            }
        }
        index ++;  // If we didn't find the pattern, we keep increasing index until either we reach the end of line of string
    }
    return NULL;
}

// Find the length of line before we reach the pattern ":X"
int findLineLength(char* buffer, char* lineName)
{
    int len = 0;
    while(buffer[len] != ':' && buffer[len] != '\0')
    {
        len++;
    }
    if(buffer[len] == '\0')
    {
        return 0;
    }
    else
    {
        if(customStrCmp(buffer + len + 1, lineName) == 0)
        {
            return len;
        }
        else
        {
            return len + 1 + findLineLength(buffer + len + 1, lineName);
        }
    }
}

// This function does the same job as strcmp, but returns 0 if str2 reaches the end (no matter if str1 is not over)
// For example "Hello, my name is greg" and "Hello" will match
int customStrCmp(char* str1, char* str2)
{
    int i = 0;
    while(str2[i] != '\0')
    {
        if(str1[i] != str2[i])
        {
            return 1;
        }
        i++;
    }
    return 0;
}
