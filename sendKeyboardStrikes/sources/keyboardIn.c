#include "./headers/keyboardIn.h"
#include <winuser.h>

BYTE releaseActionKeys();
void pressActionKeys(BYTE formerState);

// Function used to test the vKey translation
void displayPressedKeys(char c, HKL keyboardLayout)
{
    SHORT result = VkKeyScanExA(c, keyboardLayout);
    if (result == -1)
    {
        printf("Something went wrong while analysing key : %c\n", c);
        return;
    }
    else
    {
        BYTE vkCode = result & 0xFF;  // 2 lower order bytes are the keycode of the letter being pressed
        BYTE shiftState = result >> 8;  // upper order bits are used to know if ALT, SHIFT and CTRL are pressed

        printf("The complete result is %#x\n", result);
        printf("The letter code pressed is : %#x\n", vkCode);
        if (shiftState & 1) printf("SHIFT needs to be pressed\n");
        if (shiftState & 2) printf("CTRL needs to be pressed\n");
        if (shiftState & 4) printf("ALT needs to be pressed\n");
    }
    return;
}

// TODO CTRL and ALT keys do not seem to work very well ...
// CAPS LOCK and SHIFT look fine

// This function releases SHIFT, ALT, CTRL, CAPS LOCK keys if they are pressed.
// It returns a BYTE where each bit represents the state of a key before applying function
// - 1 : SHIFT
// - 2 : CTRL
// - 4 : ALT
// - 8 : CAPS LOCK
BYTE releaseActionKeys()
{
    BYTE formerState = 0x0;
    INPUT inputs[5];  // 5 is the max inputs we will send to the keyboard
    ZeroMemory(inputs, sizeof(inputs));
    if (GetKeyState(VK_SHIFT) & 0x8000)  // Check if SHIFT is holded
    {
        formerState |= 0x1;
        inputs[0].type = INPUT_KEYBOARD;
        inputs[0].ki.wVk = VK_SHIFT;
        inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;
    }
    if (GetKeyState(VK_CONTROL) & 0x8000)  // Check if CTRL is holded
    {
        formerState |= 0x2;
        inputs[1].type = INPUT_KEYBOARD;
        inputs[1].ki.wVk = VK_CONTROL;
        inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;
    }
    if (GetKeyState(VK_MENU) & 0x8000)  // Check if ALT is holded
    {
        formerState |= 0x4;
        inputs[2].type = INPUT_KEYBOARD;
        inputs[2].ki.wVk = VK_MENU;
        inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;
    }
    if (GetKeyState(VK_CAPITAL))  // Check if CAPS LOCK is toggled
    {
        formerState |= 0x8;
        inputs[3].type = INPUT_KEYBOARD;
        inputs[3].ki.wVk = VK_CAPITAL;
        // Press, and release the key
        inputs[4].type = INPUT_KEYBOARD;
        inputs[4].ki.wVk = VK_CAPITAL;
        inputs[4].ki.dwFlags = KEYEVENTF_KEYUP;
    }
    UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    if (uSent != ARRAYSIZE(inputs))
    {
        printf("SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
    }
    return formerState;
}

void pressActionKeys(BYTE formerState)
{
    INPUT inputs[5];  // 5 is the max inputs we will send to the keyboard
    ZeroMemory(inputs, sizeof(inputs));
    if (formerState & 1)  // SHIFT
    {
        inputs[0].type = INPUT_KEYBOARD;
        inputs[0].ki.wVk = VK_SHIFT;
    }
    if (formerState & 2)  // CTRL
    {
        inputs[1].type = INPUT_KEYBOARD;
        inputs[1].ki.wVk = VK_CONTROL;
    }
    if (formerState & 4)  // ALT
    {
        inputs[2].type = INPUT_KEYBOARD;
        inputs[2].ki.wVk = VK_MENU;
    }
    if (formerState & 8)  // CAPS LOCK
    {
        inputs[3].type = INPUT_KEYBOARD;
        inputs[3].ki.wVk = VK_CAPITAL;
        // Press and release
        inputs[4].type = INPUT_KEYBOARD;
        inputs[4].ki.wVk = VK_CAPITAL;
        inputs[4].ki.dwFlags = KEYEVENTF_KEYUP;
    }
    UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    if (uSent != ARRAYSIZE(inputs))
    {
        printf("SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
    }
}

void writeGivenString(char stringToWrite[], int stringLenght)
{
    HKL myLayout = GetKeyboardLayout(0);
    BYTE state = releaseActionKeys();
    printf("State of former Keyboard was : %#x\n", state);
    for (int i=0; i<stringLenght; i++)
    {
        writeCharacter(stringToWrite[i], myLayout);
    }
    pressActionKeys(state);
}

void writeCharacter(char c, HKL keyboardLayout)
{
    SHORT keyCode = VkKeyScanExA(c, keyboardLayout);
    BYTE vkCode = keyCode & 0xFF;  // 2 lower order bytes are the keycode of the letter being pressed
    BYTE shiftState = keyCode >> 8;  // higher order bits are used to know if ALT, SHIFT and CTRL are pressed

    INPUT inputs[8];  // 8 is the max inputs we will send to the keyboard
    ZeroMemory(inputs, sizeof(inputs));
    
    // Pressing keys
    if (shiftState & 1)  // We press SHIFT
    {
        inputs[0].type = INPUT_KEYBOARD;
        inputs[0].ki.wVk = VK_SHIFT;
    }
    if (shiftState & 2)  // We press CTRL
    {
        inputs[1].type = INPUT_KEYBOARD;
        inputs[1].ki.wVk = VK_CONTROL;
    }
    if (shiftState & 4)  // We press ALT
    {
        inputs[2].type = INPUT_KEYBOARD;
        inputs[2].ki.wVk = VK_MENU;
    }

    // Press the "letter" key
    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = vkCode;

    // Now we release the keys
    inputs[4].type = INPUT_KEYBOARD;
    inputs[4].ki.wVk = vkCode;
    inputs[4].ki.dwFlags = KEYEVENTF_KEYUP;

    if (shiftState & 1)  // We release SHIFT
    {
        inputs[5].type = INPUT_KEYBOARD;
        inputs[5].ki.wVk = VK_SHIFT;
        inputs[5].ki.dwFlags = KEYEVENTF_KEYUP;
    }
    if (shiftState & 2)  // We release CTRL
    {
        inputs[6].type = INPUT_KEYBOARD;
        inputs[6].ki.wVk = VK_CONTROL;
        inputs[6].ki.dwFlags = KEYEVENTF_KEYUP;
    }
    if (shiftState & 4)  // We release ALT
    {
        inputs[7].type = INPUT_KEYBOARD;
        inputs[7].ki.wVk = VK_MENU;
        inputs[7].ki.dwFlags = KEYEVENTF_KEYUP;
    }

    // send inputs to Windows
    UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    if (uSent != ARRAYSIZE(inputs))
    {
        printf("SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
    }
}

// Demo function
void SendWinKey()
{
    printf("Sending 'Win'\n");
    INPUT inputs[2];
    ZeroMemory(inputs, sizeof(inputs));

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_LWIN;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = VK_LWIN;
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

    UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    if (uSent != ARRAYSIZE(inputs))
    {
        printf("SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
    }
}
