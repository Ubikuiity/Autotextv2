#include "./sources/headers/yamlReader.h"
#include "./sources/headers/yamlWriter.h"

int main()
{
    char* filePath = "D:\\VisualStudioProjects\\Autotextv2\\submodules\\yamlHandler\\wordsTest.yaml";
    char* writeFilePath = "D:\\VisualStudioProjects\\Autotextv2\\submodules\\yamlHandler\\wordsTestBackup.yaml";

    wordPatterns* Patterns = getWordPatternsFromFile(filePath);
    displayWordPatterns(Patterns);

    appendPattern(Patterns, "Ubikuity", "Hi, my name is Ubikuity\nI am just a lambda.");

    writeFileFromWordPatterns(writeFilePath, Patterns);
    
    destroyWordPatterns(Patterns);

    wordPatterns* writtenPatterns = getWordPatternsFromFile(writeFilePath);
    displayWordPatterns(writtenPatterns);

    destroyWordPatterns(writtenPatterns);

    return 0;
}