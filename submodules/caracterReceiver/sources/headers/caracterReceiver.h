#pragma once

#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <process.h>
#include <windows.h>
#include <string.h>

#define DEFAULT_PORT "7773"
#define MAX_RECEIVER_PAQUET_SIZE 5
#define STOP_RECEIVER_SIGNAL "STOP" // This string must be 2 caracters at least and 4 caracters max

typedef struct threadProperties threadProperties;
struct threadProperties
{
    SOCKET* Listener;
    HANDLE mutexHandle;
    void (*callback)(char*);
    char ChrBuffer[MAX_RECEIVER_PAQUET_SIZE];
};

// Public functions

threadProperties* StartReceiverAsThread(void (*callback)(char*), int verbose);
void StopReceiver(threadProperties* propertiesOfThread);

// Private functions

SOCKET* startReceiver(int verbose);
void waitForCharacter(SOCKET* ListenSocketPtr, char* cPtr, int verbose);
void encapsulatedWaitForCharacter(void* vparams);
void clearReceiver(SOCKET* ListenSocketPtr);
int sendStopSignal();
