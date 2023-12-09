#pragma once
#include <windows.h>
#include <winuser.h>
#include <stdio.h>
#include <string.h>

#define HANDLE_CTRLALTSHIFT_KEYS FALSE  // This variable handle the shift alt ctrl relasing when we type
// it is not very functionnal so should stay disable

// Public functions

void writeCharacter(char c, HKL keyboardLayout);
void writeGivenString(char stringToWrite[], int stringLenght);
void sendBackspacesInputs(int n);

// Private functions

void displayPressedKeys(char c, HKL keyboardLayout);
