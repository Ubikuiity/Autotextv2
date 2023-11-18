#pragma once

#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

SOCKET* startReceiver(int verbose);
void waitForCharacter(SOCKET* ListenSocketPtr, char* cPtr, int verbose);
void stopReceiver(SOCKET* ListenSocketPtr);
