#ifndef ZFB_KEY_INPUT_H
#define ZFB_KEY_INPUT_H

#ifdef __WIN32
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
#endif

void ZFB_RawMode();
void ZFB_Exit_RawMode();
void ZFB_ProcessKeyboard();
#endif