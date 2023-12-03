#include "./sources/headers/yamlHandler.h"

int main()
{
    char* filePath = "D:\\VisualStudioProjects\\Autotextv2\\submodules\\yamlHandler\\wordsTest.yaml";

    char* buf = fileToString(filePath);

    wordPatterns* Patterns = findWordsAndReplacements(buf);

    displayWordPatterns(Patterns);

    destroyWordPatterns(Patterns);

    return 0;
}