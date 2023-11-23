#include "./headers/caracterReceiver.h"

// Creates character receiver. port defaults to 7773 if 0 is given as input
// verbose parameter is 0 for no verbose and 1 for debug verbose
SOCKET* startReceiver(int verbose)
{
    // Initialisation de la librairie Winsock
    WSADATA wsaData;
    
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);  // (Version 2,2 de Winsock)
    if (iResult != 0) {
        if (verbose)
        {
            printf("WSAStartup failed: %d\n", iResult);
        }
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
    if (iResult != 0) {
        if (verbose)
        {
            printf("getaddrinfo failed: %d\n", iResult);
        }
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
        if (verbose)
        {
            printf("bind failed with error: %d\n", WSAGetLastError());
        }
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
    else if(verbose)
    {
        printf("Received packet : %s\n", cPtr);
    }

    return;
}

void stopReceiver(SOCKET* ListenSocketPtr)
{
    closesocket(*ListenSocketPtr);
    free(ListenSocketPtr);
    WSACleanup();

    return;
}
