#pragma once

#include <fstream>
#include <string>
#include <filesystem>
#include <chrono>
#include <ctime>
#include <iostream>

// Basic custom class used for logging
class Logger
{
    private:
    std::ofstream fileStream;

    public:
    Logger(std::string filePath)
    {
        this->fileStream.open(filePath, std::ofstream::trunc);  // Recreates file from scratch
    }

    ~Logger()
    {
        this->fileStream.close();
    }

    void log(std::string content);
};

void GetLogfileName(std::string*);
int clearOldLogs(const std::string logFolder, int nbDays);
