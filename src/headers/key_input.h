#ifndef ZFB_KEY_INPUT_H
#define ZFB_KEY_INPUT_H

#define INPUT_PATH "/dev/input/"
#define DEBUG 1 // Set to 1 to enable debug messages

#ifdef _WIN32
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <linux/input.h> // for KEY_ESC etc.
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <sys/ioctl.h>
    #include <dirent.h>
    #include <errno.h>
#endif

void ZFB_InitInput();              // Detects and opens the keyboard device
void ZFB_CloseInput();             // Closes the device
void ZFB_ProcessKeyboard();        // Reads events and pushes ZFB_Events
int  ZFB_IsKeyPressed(int key);    // Returns whether key is currently held down
void ZFB_RawMode();
void ZFB_ExitRawMode();
#endif // ZFB_KEY_INPUT_H
