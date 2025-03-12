#include "../headers/key_input.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#ifdef _WIN32
#include <windows.h>

void ZFB_RawMode() {}
void ZFB_Exit_RawMode() {}

int ZFB_KeyPressed() {
    for (int key = 9; key < 256; key++) { // Check the keys
        if (GetAsyncKeyState(key) & 0x8000) {
            return key;
        }
    }
    return 0;
}

#else // Linux / Unix Implementation

static struct termios oldt;

void ZFB_RawMode(); {
    struct termios newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO | ISIG); // RAW MODE: No buffering, no echo, no signals
    newt.c_iflag &= ~(IXON | ICRNL); // Disable flow control and carriage return mapping
    newt.c_oflag &= ~(OPOST); // Disable output processing
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    printf("\e[?25l");
    fflush(stdout);
}

void ZFB_Exit_RawMode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore Settings
    printf("\e[?25h");
    fflush(stdout);
}

int ZFB_KeyPressed() {
    int ch;
    int oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK); // Nonblocking input
    
    ch = getchar();

    fcntl(STDIN_FILENO, F_SETFL, oldf); // Restore flags
    return (ch != EOF) ? ch : 0;
}

#endif
