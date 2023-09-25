#include <Windows.h>
#include <string.h>
#include <ctime>
#include <iostream>

using namespace std;

void getTime(string*);

int main(/* int argc, char* argv[] */)
{
    // FreeConsole(); Let go of the console
    string filename;  // name of file where we are going to log
    getTime(&filename);
    cout << "Logging to file : " << filename << endl;
    return 0;
}

void getTime(string* ptrFilename)
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