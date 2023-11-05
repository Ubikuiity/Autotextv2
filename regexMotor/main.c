#include "sources/headers/createMotor.h"

int main ()
{
    char* myWords[] = {"Hi"};
    createMotor(myWords, sizeof(myWords) / sizeof(myWords[0]));
    
    return 0;
}