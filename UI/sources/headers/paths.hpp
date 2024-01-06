#pragma once

#define PATHS_LOGFOLDER "log"
#define PATHS_RESFOLDER "res"
#define PATHS_WORDSFILE "words.yaml"
#define PATHS_ICONFILE "icon.ico"

#include <string>
#include <Windows.h>
#include <codecvt>
#include <locale>

// Returns the folder that contains the exe
std::string getPathOfExeAsString();
