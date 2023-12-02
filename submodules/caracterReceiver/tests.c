#include "./sources/headers/caracterReceiver.h"


// This parts tests the receiver with a thread
// We send a "STOP" UDP signal to the thread to end it

void printString(char* readCaracters, void* params);

int main()
{
    int duration = 10;  // 20 sec

    threadProperties* myThread = StartReceiverAsThread(printString, NULL, 1);

    printf("Waiting for enter input to stop the receiver\n");

    getchar();
    printf("Stopping receiver\n");
    StopReceiver(myThread);
    printf("Receiver stopped, ending program\n");

    return 0;
}

// Callback function that simply prints the string
void printString(char* readCaracters, void* params)
{
    printf("Received caracter %s\n", readCaracters);
}
