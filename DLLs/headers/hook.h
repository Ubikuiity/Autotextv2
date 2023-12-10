#pragma once

#pragma comment(lib, "Ws2_32.lib")

#include <fstream>
#include <WinUser.h>

#ifdef KEYBOARDHOOK_EXPORTS
#define KEYBOARDHOOK_API __declspec(dllexport)
#else
#define KEYBOARDHOOK_API __declspec(dllimport)
#endif

typedef void (*YOUR_CALLBACK)(char myChar);
extern "C" KEYBOARDHOOK_API BOOL installhook();

extern "C" KEYBOARDHOOK_API BOOL UnHook();
