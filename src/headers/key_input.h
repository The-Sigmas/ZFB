#ifndef ZFB_KEY_INPUT_H
#define ZFB_KEY_INPUT_H

#ifdef __WIN32
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
#endif

void ZFB_InitKeyboard();
void ZFB_CloseKeyboard();
int ZFB_KeyPressed();

#endif