#pragma once

#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define DEFAULT_PORT "7773"
#define MAX_RECEIVER_PAQUET_SIZE 5

SOCKET* startReceiver(int verbose);
void waitForCharacter(SOCKET* ListenSocketPtr, char* cPtr, int verbose);
void stopReceiver(SOCKET* ListenSocketPtr);
