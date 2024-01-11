#include "./sources/headers/logger.hpp"
#include <iostream>

int main()
{
    std::string filename;  // name of file where we are going to log
    GetLogfileName(&filename);
    std::string prefix = "D:\\VisualStudioProjects\\Autotextv2\\output\\log";
    // std::cout << "Logging to file : " << prefix << filename << std::endl;

    // Logger mainLogger(prefix + "\\" + filename);  // Hardcoded path here !
    // std::cout << "Logger opened" << std::endl;
    
    // mainLogger.log("Testing logging");

    clearOldLogs(prefix, 2);

    // std::cout << "End of execution" <<std::endl;
    return 0;
}