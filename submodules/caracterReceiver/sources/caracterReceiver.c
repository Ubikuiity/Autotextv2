#include "./headers/caracterReceiver.h"

// Creates character receiver. port defaults to 7773 if 0 is given as input
// verbose parameter is 0 for no verbose and 1 for debug verbose
SOCKET* startReceiver(int verbose)
{
    // Initialisation de la librairie Winsock
    WSADATA wsaData;
    
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);  // (Version 2,2 de Winsock)
    if (iResult != 0) {
        if (verbose) printf("WSAStartup failed: %d\n", iResult);
        return NULL;
    }

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    ZeroMemory(&hints, sizeof (hints));  // Fills the memory with zeros
    hints.ai_family = AF_INET;  // IPv4 address family
    hints.ai_socktype = SOCK_DGRAM;  // Datagram socket -> Used in UDP
    hints.ai_protocol = IPPROTO_UDP;  // Protocol UDP
    hints.ai_flags = AI_PASSIVE;  // Socket address will be used in bind()

    // Resolve the local address and port to be used by the server
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0)
    {
        if (verbose) printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return NULL;
    }
    else if (verbose)
    {
        printf("Succes initializing information of socket address !\n");
    }

    SOCKET *ListenSocketPtr = malloc(sizeof(SOCKET));
    *ListenSocketPtr = INVALID_SOCKET;

    // Create a SOCKET for the server to listen for client connections
    *ListenSocketPtr = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (*ListenSocketPtr == INVALID_SOCKET)
    {
        if (verbose)
        {
            printf("Error at socket(): %ld\n", WSAGetLastError());
        }
        freeaddrinfo(result);
        WSACleanup();
        return NULL;
    }
    else if(verbose)
    {
        printf("Socket initialized !\n");
    }

    // Now binding the socket to the address defined earlier
    iResult = bind(*ListenSocketPtr, result->ai_addr, (int)result->ai_addrlen);
    
    if (iResult == SOCKET_ERROR) {
        if (verbose) printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(*ListenSocketPtr);
        WSACleanup();
        return NULL;
    }

    // Once socket is binded, we don't need the address infos anymore
    freeaddrinfo(result);

    return ListenSocketPtr;
}

// This function is blocking until we get a caracter from a sender to our socket.
// cPtr must link to an array of char of size at least 5
// verbose parameter is 0 for no verbose and 1 for debug verbose
void waitForCharacter(SOCKET* ListenSocketPtr, char* cPtr, int verbose)
{
    // Call the recvfrom function to receive datagrams
    // on the bound socket.
    int bufLen = MAX_RECEIVER_PAQUET_SIZE;

    SOCKET ListenSocket = *ListenSocketPtr;

    struct sockaddr_in SenderAddr;  // Structure used to get sender address
    int SenderAddrSize = sizeof (SenderAddr);

    int iResult = recvfrom(ListenSocket,
                       cPtr, bufLen, 0,
                       (SOCKADDR *) &SenderAddr, &SenderAddrSize);
    if (iResult == SOCKET_ERROR && verbose) {
        printf("recvfrom failed with error %d\n", WSAGetLastError());
    }
    else if (verbose)
    {
        printf("Received packet : %s\n", cPtr);
    }
    return;
}

// encapsulated version ready to be sent to _beginthread
void encapsulatedWaitForCharacter(void* vparams)
{
    time_t lastBufferClearanceTime = 0;
    threadProperties* params = (threadProperties*) vparams;  // Casting type
    WaitForSingleObject(params->mutexHandle, INFINITE);  // TODO We should add finite time and check that we indeed got the mutex
    while(TRUE)
    {
        waitForCharacter(params->Listener, params->ChrBuffer, 0);
        // If last time we asked to clear the buffer was 1 second or less, skip the paquets
        if ((time(NULL)-lastBufferClearanceTime) < BUFFER_CLEARANCE_DURATION)
        {
            continue;
        }  // else we process the paquet
        if (!strcmp(params->ChrBuffer, STOP_RECEIVER_SIGNAL))  // If we receive STOP, we stop
        {
            break;
        }
        else  // Else we call callback function
        {
            if (params->callback(params->ChrBuffer, params->paramsForCallback) == 1)  // This calls the callback
            {
                lastBufferClearanceTime = time(NULL);  // We set 
            }
        }
    }

    ReleaseMutex(params->mutexHandle);
}

void clearReceiver(SOCKET* ListenSocketPtr)
{
    closesocket(*ListenSocketPtr);
    free(ListenSocketPtr);
    WSACleanup();

    return;
}

// Creates the caracter receiver as a thread.
// This thread will call the callback with params for callback and a given caracter each time we press the keyboard key
threadProperties* StartReceiverAsThread(int (*callback)(char*, void*), void* paramsForCallback, int verbose)
{
    threadProperties* thrdProps = malloc(sizeof(threadProperties));
    
    if (verbose) printf("Creating Receiver...\n");

    thrdProps->Listener = startReceiver(0);  // Create listener
    if (thrdProps->Listener == NULL)
    {
        if (verbose) printf("Something went wrong during socket creation :(\n");
        return NULL;
    }
    if (verbose) printf("Receiver OK\n\n");

    thrdProps->mutexHandle = CreateMutex(NULL, FALSE, NULL);
    thrdProps->callback = callback;
    thrdProps->paramsForCallback = paramsForCallback;
    _beginthread(encapsulatedWaitForCharacter, 0, (void*)thrdProps);
    return thrdProps;
}

// Used to stop the receiver by sending a STOP_RECEIVER_SIGNAL package
// WSAStartup must have already been called before calling this function
// (Which should be the case if receiver is up)
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

// This function closes the threads and clear memory.
// it needs to be called with the mutex handle of returned by the thread when created
void StopReceiver(threadProperties* propertiesOfThread)
{
    sendStopSignal();
    // Wait for receiver to be closed, and then destroy the object
    DWORD res = WaitForSingleObject(propertiesOfThread->mutexHandle, 1000);
    while (res != WAIT_OBJECT_0)  // We send request to close the socket every second if it doesn't close properly
    {
        printf("Receiver not stopped yet ... Error : %#010x/", res);
        res = WaitForSingleObject(propertiesOfThread->mutexHandle, 1000);
    }
    
    clearReceiver(propertiesOfThread->Listener);
    CloseHandle(propertiesOfThread->mutexHandle);
    free(propertiesOfThread);
}
