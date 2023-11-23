#include "./sources/headers/caracterReceiver.h"
#include <windows.h>
#include <string.h>
#include <process.h>

#define STOP_RECEIVER_SIGNAL "STOP" // This string must be 2 caracters at least and 4 caracters max

// This parts tests the receiver with a thread
// We send a "STOP" UDP signal to the thread to end it

typedef struct paramsForReceiver paramsForReceiver;
struct paramsForReceiver
{
    SOCKET *ListenSocketPtr;
    char *cPtr;
    void (*callback)(char*);  // Callback being called with string read from socket
    HANDLE mutexHandle;  // Mutex used to signal that the thread has been successfully stoped
};

void encapsulatedWaitForCharacter(void* vparams);
int sendStopSignal();
void printString(char* readCaracters);

int main()
{
    HANDLE receiverRunningMutex;  // Mutex used to wait for thread end
    int duration = 20;  // 20 sec

    printf("Creating Receiver...\n");
    SOCKET* Listener = startReceiver(0);  // Create listener
    if (Listener == NULL)
    {
        printf("Something went wrong during socket creation :(\n");
        return 1;
    }
    printf("Receiver OK\n\n");

    receiverRunningMutex = CreateMutex(NULL, FALSE, NULL);

    char caracterBuffer[MAX_RECEIVER_PAQUET_SIZE];
    paramsForReceiver params = {Listener, caracterBuffer, printString, receiverRunningMutex};
    _beginthread(encapsulatedWaitForCharacter, 0, (void*)&params);

    printf("Will send receiver signal to stop in 20 seconds...\n");
    Sleep(duration*1000);
    sendStopSignal();
    printf("Sent stop signal, waiting for receiver to receive it...\n");

    // Wait for receiver to be closed, and then destroy the object
    WaitForSingleObject(receiverRunningMutex, INFINITE);
    stopReceiver(Listener);
    CloseHandle(receiverRunningMutex);
    printf("Receiver closed\n\n");

    return 0;
}

void encapsulatedWaitForCharacter(void* vparams)
{
    paramsForReceiver* params = (paramsForReceiver*) vparams;  // Casting type
    WaitForSingleObject(params->mutexHandle, INFINITE);  // TODO We should add finite time and check that we indeed got the mutex
    while(TRUE)
    {
        waitForCharacter(params->ListenSocketPtr, params->cPtr, 0);
        if (!strcmp(params->cPtr, STOP_RECEIVER_SIGNAL))  // If we receive STOP, we stop
        {
            break;
        }
        else
        {
            params->callback(params->cPtr);  // Else we call callback function
        }
    }

    ReleaseMutex(params->mutexHandle);
}

// Callback function that simply prints the string
void printString(char* readCaracters)
{
    printf("Received caracter %s\n", readCaracters);
}

// WSAStartup must have already been called before calling this function
int sendStopSignal()
{
    struct addrinfo hints;

    ZeroMemory(&hints, sizeof (hints));  // Fills the memory with zeros
    hints.ai_family = AF_INET;  // IPv4 address family
    hints.ai_socktype = SOCK_DGRAM;  // Datagram socket -> Used in UDP
    hints.ai_protocol = IPPROTO_UDP;  // Protocol UDP
    hints.ai_flags = AI_PASSIVE;  // Socket address will be used in bind()

    SOCKET SendingSocket = INVALID_SOCKET;

    // Create a SOCKET for client to send data
    SendingSocket = socket(hints.ai_family, hints.ai_socktype, hints.ai_protocol);
    if (SendingSocket == INVALID_SOCKET)
    {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Call the send function to send datagrams
    char sendBuffer[5] = STOP_RECEIVER_SIGNAL;
    int bufLen = 5;

    struct sockaddr_in ReceiverAddr;
    ReceiverAddr.sin_family = AF_INET;
    ReceiverAddr.sin_port = htons(7773);  // warning Hardcoded to test here
    ReceiverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int iResult = sendto(SendingSocket,
                       sendBuffer, bufLen, 0,
                       (SOCKADDR *) &ReceiverAddr, sizeof(ReceiverAddr));
    if (iResult == SOCKET_ERROR) {
        printf("sendto failed with error %d\n", WSAGetLastError());
    }

    closesocket(SendingSocket);

    return 0;
}