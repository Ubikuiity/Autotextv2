#include "./headers/logger.hpp"

using namespace std;

// Logs into the file (Will add timestamps)
void Logger::log(string content)
{
    time_t currentTime;
    currentTime = time(NULL);
    tm* now = localtime(&currentTime);
    // Adds time to log
    this->fileStream << to_string(now->tm_hour) << "°" << to_string(now->tm_min) << "\'" << to_string(now->tm_sec) << "\'\'" << " --- ";
    this->fileStream << content << endl;
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
        + to_string(now->tm_mon + 1) + "_"
        + to_string(now->tm_mday) + "_"
        + to_string(now->tm_hour) + "h"
        + to_string(now->tm_min) + "m"
        + to_string(now->tm_sec) + "s"
        + ".log";
}

// Clear logs older than X days
// Returns number of files cleared
int clearOldLogs(const std::string logFolder, int nbDays)
{
    int cmp = 0;
    for (filesystem::directory_entry const& dir_entry : filesystem::directory_iterator{logFolder}) 
    {
        auto modifiedTime = filesystem::last_write_time(dir_entry.path());  // Gets the last modified time
        auto currentTime = filesystem::file_time_type::clock::now();

        // Magic casting to get hour difference between 2 times
        int sinceEpochModifiedTime = chrono::duration_cast<std::chrono::hours>(modifiedTime.time_since_epoch()).count();
        int sinceEpochCurrentTime = chrono::duration_cast<std::chrono::hours>(currentTime.time_since_epoch()).count();

        int comparedTime = sinceEpochCurrentTime - sinceEpochModifiedTime;
        if (comparedTime > nbDays * 24)
        {
            filesystem::remove(dir_entry.path());
            cmp++;
        }
    }
    return cmp;
}
