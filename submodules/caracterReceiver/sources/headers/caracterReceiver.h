#pragma once

#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <process.h>
#include <windows.h>
#include <string.h>
#include <time.h>

#define DEFAULT_PORT "7773"
#define MAX_RECEIVER_PAQUET_SIZE 5
#define STOP_RECEIVER_SIGNAL "STOP" // This string must be 2 caracters at least and 4 caracters max
#define BUFFER_CLEARANCE_DURATION 0.5  // This may need to be increased for very long outputs

typedef struct threadProperties threadProperties;
struct threadProperties
{
    SOCKET* Listener;
    HANDLE mutexHandle;
    int (*callback)(char*, void*);  // The int returned is an indicator if we need to clear the buffer or not
    void* paramsForCallback;
    char ChrBuffer[MAX_RECEIVER_PAQUET_SIZE];
};

// Public functions

threadProperties* StartReceiverAsThread(int (*callback)(char*, void*), void* paramsForCallback, int verbose);
void StopReceiver(threadProperties* propertiesOfThread);

// Private functions

SOCKET* startReceiver(int verbose);
void waitForCharacter(SOCKET* ListenSocketPtr, char* cPtr, int verbose);
void encapsulatedWaitForCharacter(void* vparams);
void clearReceiver(SOCKET* ListenSocketPtr);
int sendStopSignal();
