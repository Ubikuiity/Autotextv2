#include "./submodules/logger/sources/headers/logger.h"
#include "./submodules/regexMotor/sources/headers/useMotor.h"

#include "./DLLHeader/hook.h"
#include <iostream>

using namespace std;

int main(/* int argc, char* argv[] */)
{
    string filename;  // name of file where we are going to log
    GetLogfileName(&filename);
    cout << "Logging to file : " << filename << endl;

    Logger mainLogger("output\\log\\" + filename);  // Hardcoded path here !
    mainLogger.log("Started Logging");

    

    return 0;
}
