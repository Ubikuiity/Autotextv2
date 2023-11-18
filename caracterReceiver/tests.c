#include "./sources/headers/caracterReceiver.h"
#include <time.h>

int main()
{
    int duration = 20;  // 20 sec
    time_t startTime;
    startTime = time(NULL);

    printf("Creating Receiver...\n");
    SOCKET* Listener = startReceiver(0);  // Create listener
    if (Listener == NULL)
    {
        printf("Something went wrong during socket creation :(\n");
        return 1;
    }
    printf("Receiver OK\n\n");

    time_t currentTime;
    currentTime = time(NULL);
    while(currentTime - startTime < duration)
    {
        char c;
        waitForCharacter(Listener, &c, 0);
        printf("Received caracter %c\n", c);

        currentTime = time(NULL);  // This doesn't work because we are stuck in the blocking waitForCaracter function
    }

    printf("End of while loop, deleting receiver...\n");
    stopReceiver(Listener);
    printf("Receiver closed\n\n");

    return 0;
}