extern "C"
{
    #include "./submodules/regexMotor/sources/headers/useMotor.h"
    #include "./submodules/caracterReceiver/sources/headers/caracterReceiver.h"
    #include "./submodules/sendKeyboardStrikes/sources/headers/keyboardIn.h"
}
#include "./submodules/logger/sources/headers/logger.h"

#include "./DLLs/headers/hook.h"

#include <iostream>

using namespace std;

void nextStepWrapper(char* c, void* param);

int main(/* int argc, char* argv[] */)
{
    string filename;  // name of file where we are going to log
    GetLogfileName(&filename);
    string prefix = "D:\\VisualStudioProjects\\Autotextv2\\output\\log";  // Hardcoded path here
    Logger mainLogger(prefix + "\\" + filename);  // Creating Logger
    cout << "Logging to file : " << prefix << "\\" << filename << endl;
    mainLogger.log("> Started Logging <");

    char* myWords[] = {"hello", "cat", "slay", "slice", "slicat", "snake", "hi", "sheo"};

    reMotor* myMotor = createMotor(myWords, sizeof(myWords) / sizeof(myWords[1]));
    mainLogger.log("Motor created");

    installhook();
    mainLogger.log("Keyboard hook injected");

    threadProperties* myReceiver = StartReceiverAsThread(nextStepWrapper, (void*) myMotor, 0);
    mainLogger.log("Receiver started with motor step as callback");

    std::cout << "Waiting for user input to stop ..." << std::endl;
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
void nextStepWrapper(char* c, void* param)
{
    reMotor* motor = (reMotor*) param;  // Casting argument to correct type
    nextStep(motor, *c);  // Calls next step
    plotMotor(motor);  // Display motor
    return;
}
