#ifndef ZFB_KEY_INPUT_H
#define ZFB_KEY_INPUT_H

#ifdef _WIN32
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <linux/input.h> // for KEY_ESC etc.
#endif

void ZFB_InitInput();              // Detects and opens the keyboard device
void ZFB_CloseInput();             // Closes the device
void ZFB_ProcessKeyboard();        // Reads events and pushes ZFB_Events
int  ZFB_IsKeyPressed(int key);    // Returns whether key is currently held down
#endif // ZFB_KEY_INPUT_H
