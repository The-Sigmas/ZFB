#include "key_input.h"
#include "event.h"
#include <sys/select.h>   /* select() */

static struct termios oldt;
static int key_states[KEY_MAX] = {0}; 
static int keyboard_fd = -1; // Global file descriptor for keyboard

void ZFB_RawMode() {
    // Save the old terminal settings
    tcgetattr(STDIN_FILENO, &oldt);
    
    struct termios newt;
    tcgetattr(STDIN_FILENO, &newt);
    newt.c_lflag &= ~(ICANON | ECHO | ISIG);
    newt.c_iflag &= ~(IXON | ICRNL);
    newt.c_oflag &= ~(OPOST);
    newt.c_cc[VMIN] = 1;
    newt.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
    printf("\e[?25l\r");
    fflush(stdout);
}

void ZFB_ExitRawMode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    printf("\e[?25h\r");
    fflush(stdout);
}

int ZFB_Detect_ActiveKeyboard() {
    const int MAX_DEVS = 64;
    char path[64];
    int fds[MAX_DEVS];
    int count = 0;

    // Open all event devices non-blocking
    for (int i = 0; i < MAX_DEVS; ++i) {
        snprintf(path, sizeof(path), "/dev/input/event%d", i);
        int fd = open(path, O_RDONLY | O_NONBLOCK);
        if (fd >= 0) {
#if DEBUG
            printf("Opened %s\r\n", path);
#endif
            fds[count++] = fd;
        }
    }

    printf("Press any key to detect your active keyboard...\r\n");

    while (1) {
        fd_set readfds;
        FD_ZERO(&readfds);
        int maxfd = -1;

        for (int i = 0; i < count; ++i) {
            FD_SET(fds[i], &readfds);
            if (fds[i] > maxfd) maxfd = fds[i];
        }

        if (select(maxfd + 1, &readfds, NULL, NULL, NULL) > 0) {
            for (int i = 0; i < count; ++i) {
                if (FD_ISSET(fds[i], &readfds)) {
                    struct input_event ev;
                    if (read(fds[i], &ev, sizeof(ev)) > 0 && ev.type == EV_KEY) {
                        printf("Detected input on /dev/input/event%d\r\n", i);
                        // Close all other fds
                        for (int j = 0; j < count; ++j) {
                            if (j != i) close(fds[j]);
                        }
                        return fds[i];
                    }
                }
            }
        }
    }

    return -1; // Should never hit here
}

void ZFB_InitInput() {
    keyboard_fd = ZFB_Detect_ActiveKeyboard();
    if (keyboard_fd < 0) {
        fprintf(stderr, "[ERROR] Failed to detect keyboard input device.\r\n");
        exit(1);
    } else if (DEBUG) {
        printf("[DEBUG] Keyboard initialized on fd=%d\r\n", keyboard_fd);
    }
}

void ZFB_CloseInput() {
    if (keyboard_fd >= 0) {
        close(keyboard_fd);
        keyboard_fd = -1;
        if (DEBUG) printf("[DEBUG] Keyboard input closed.\r\n");
    }
}

void ZFB_ProcessKeyboard() {
    if (keyboard_fd == -1) return;

    struct input_event event;
    while (read(keyboard_fd, &event, sizeof(event)) > 0) {
        if (event.type == EV_KEY) {
            int code = event.code;
            int value = event.value;

            if (code < 0 || code >= KEY_MAX) continue;

            if (value == 1 && key_states[code] == 0) {
                // Key down
                key_states[code] = 1;
                ZFB_Event down_event = {
                    .type = ZFB_EVENT_KEYDOWN,
                    .data.key.key_code = code
                };
                ZFB_PushEvent(&down_event);
            } else if (value == 0 && key_states[code] == 1) {
                // Key up
                key_states[code] = 0;
                ZFB_Event up_event = {
                    .type = ZFB_EVENT_KEYUP,
                    .data.key.key_code = code
                };
                ZFB_PushEvent(&up_event);
            }

            // Ignore value == 2 (key repeat)
        }
    }
}

int ZFB_IsKeyPressed(int key) {
    if (key < 0 || key >= KEY_MAX) return 0;
    return key_states[key];
}
