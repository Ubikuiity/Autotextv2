#pragma once
#include <windows.h>
#include <winuser.h>
#include <stdio.h>
#include <string.h>

void displayPressedKeys(char c, HKL keyboardLayout);
void writeCharacter(char c, HKL keyboardLayout);
void writeGivenString(char stringToWrite[], int stringLenght);
