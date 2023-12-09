#include <stdio.h>
#include <time.h>

int main()
{
    time_t startProgram = 0;
    time_t now = time(NULL);
    printf("Starting time is : %d\nCurrent time is : %d", startProgram, now);
    while((now - startProgram) < 1)
    {
        now = time(NULL);
        printf("Starting time is : %i\nCurrent time is : %i", startProgram, now);
    }
    return 0;
}