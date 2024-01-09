#pragma once

#define PATHS_LOGFOLDER "log"
#define PATHS_RESFOLDER "res"
#define PATHS_WORDSFILE "words.yaml"
#define PATHS_ICONFILE "icon.ico"
#define PATHS_PROGRAMNAME "Autotext"

#include <string>
#include <Windows.h>
#include <Shlobj.h>
#include <codecvt>
#include <locale>
#include <filesystem>
// #include <Objbase.h>
// #include <Knownfolders.h>
// #include <iostream>

// Returns the folder that contains the exe
std::string getPathOfExeAsString();
std::string getAppdataAsString();
int createAppDataRessourcesIfNotExistent();
