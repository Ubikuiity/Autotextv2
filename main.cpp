extern "C"
{
    #include "./submodules/regexMotor/sources/headers/useMotor.h"
    #include "./submodules/caracterReceiver/sources/headers/caracterReceiver.h"
    #include "./submodules/sendKeyboardStrikes/sources/headers/keyboardIn.h"
    #include "./submodules/yamlHandler/sources/headers/all.h"
}
#include "./submodules/logger/sources/headers/logger.h"

#include "./DLLs/headers/hook.h"

#include <iostream>

using namespace std;

typedef struct paramsForCallback paramsForCallback;
struct paramsForCallback
{
    reMotor* motor;
    wordPatterns* patterns;
};

int callbackWrapper(char* c, void* params);

int main(/* int argc, char* argv[] */)
{
    string filename;  // name of file where we are going to log
    GetLogfileName(&filename);
    string prefix = "D:\\VisualStudioProjects\\Autotextv2\\output\\log";  // Hardcoded path here
    Logger mainLogger(prefix + "\\" + filename);  // Creating Logger
    cout << "Logging to file : " << prefix << "\\" << filename << endl;
    mainLogger.log("> Started Logging <");

    mainLogger.log("Reading content of yaml input file");

    char yamlWordPath[] = "D:\\VisualStudioProjects\\Autotextv2\\wordsTest.yaml";
    wordPatterns* myPatterns = getWordPatternsFromFile(yamlWordPath);

    mainLogger.log("Data read successfully !");

    reMotor* myMotor = createMotor(myPatterns->words);
    mainLogger.log("Motor created");

    paramsForCallback* myParams = (paramsForCallback *) malloc(sizeof(paramsForCallback));
    myParams->motor = myMotor;
    myParams->patterns = myPatterns;

    installhook();
    mainLogger.log("Keyboard hook injected");

    threadProperties* myReceiver = StartReceiverAsThread(callbackWrapper, (void*) myParams, 0);
    mainLogger.log("Receiver started with motor step as callback");

    std::cout << "Waiting for enter user input to stop program ..." << std::endl;
    getchar();
    std::cout << "Received stopping input" << std::endl;

    mainLogger.log("Clearing memory ...");

    StopReceiver(myReceiver);
    mainLogger.log("Destroyed Receiver");

    UnHook();
    mainLogger.log("Removed Hook");

    destroyMotor(myMotor);
    mainLogger.log("Destroyed regex Motor");

    mainLogger.log("> Exiting program <");
    return 0;
}

// Wrapper for nextStep function to send it to thread as a callback
int callbackWrapper(char* c, void* params)
{
    paramsForCallback* paramsCasted = (paramsForCallback*) params;  // Casting argument to correct type
    reMotor* motor = paramsCasted->motor;
    wordPatterns* patterns = paramsCasted->patterns;

    if(*c == '\b')  // Handle the special case of backspaces
    {
        undoLastStep(motor);
        plotMotor(motor);  // Display motor
        return 0;
    }

    nextStep(motor, *c);  // Calls next step
    plotMotor(motor);  // Display motor
    if (int finalStateIndex = checkAndGetFinals(motor))  // If we hit a final value
    {
        int convertedStateIndex = getIndexOfValue(motor->FinalStates, finalStateIndex);
        char* wordDetected = getStrListeValue(motor->FinalStrings, convertedStateIndex);
        char* replacer = getReplacerFromWord(patterns, wordDetected);
        sendBackspacesInputs(strlen(wordDetected));
        writeGivenString(replacer, strlen(replacer));

        restartMotorForNextRun(motor);
        return 1;
    }
    return 0;
}
