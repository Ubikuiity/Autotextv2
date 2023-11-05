#include "./headers/logger.h"

// Logs into the file (Will add timestamps)
void Logger::log(std::string content)
{
    time_t currentTime;
    currentTime = time(NULL);
    tm* now = localtime(&currentTime);
    // Adds time to log
    this->fileStream << std::to_string(now->tm_hour) << "°" << std::to_string(now->tm_min) << "\'" << std::to_string(now->tm_sec) << "\'\'" << " --- ";
    this->fileStream << content << std::endl;
}

/**
* Get current time and returns a formated string. ex : "9_25_22h23m47s.log"
*/
void GetLogfileName(std::string* ptrFilename)
{
    time_t currentTime;
    currentTime = time(NULL);
    // cout << "Current timestamp is : " << currentTime << endl;
    tm* now = localtime(&currentTime);
    // cout << "Current time is : " << now->tm_hour << "h" << now->tm_min << "m" << now->tm_sec << "s" << endl;
    *ptrFilename = ""
        + std::to_string(now->tm_mon + 1) + "_"
        + std::to_string(now->tm_mday) + "_"
        + std::to_string(now->tm_hour) + "h"
        + std::to_string(now->tm_min) + "m"
        + std::to_string(now->tm_sec) + "s"
        + ".log";
}
