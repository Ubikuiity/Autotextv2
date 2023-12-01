#include "./sources/headers/keyboardIn.h"

int main()
{
    char myChar = '\n';
    HKL myLayout = GetKeyboardLayout(0);

    displayPressedKeys(myChar, myLayout);

    char myString[] = "Hello, This is my test string @@@@@_ECHO\nHELLO, THIS IS MY TEST STRING...";
    printf("In 5 seconds, we will type : %s\n", myString);
    Sleep(5000);

    printf("Typing the string ...\n");
    writeGivenString(myString, strlen(myString));

    return 0;
}
