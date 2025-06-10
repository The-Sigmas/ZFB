#include "../../headers/event.h"
#include "../../headers/key_input.h"
#include <sys/select.h> /* select() */

#ifndef NLONGS
#define NLONGS(n) (((n) + (sizeof(long) * 8) - 1) / (sizeof(long) * 8))
#endif
#ifndef BIT_WORD
#define BIT_WORD(n) ((n) / (sizeof(long) * 8))
#endif
#ifndef BIT_SHIFT
#define BIT_SHIFT(n) ((n) % (sizeof(long) * 8))
#endif

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

static int is_keyboard(int fd) {
  unsigned long ev_bits[NLONGS(EV_MAX)];
  unsigned long key_bits[NLONGS(KEY_MAX)];
  if (ioctl(fd, EVIOCGBIT(0, sizeof(ev_bits)), ev_bits) < 0) {
    return 0;
  }

  // Check if the device supports key/button events (EV_KEY)
  if (!((ev_bits[BIT_WORD(EV_KEY)] >> BIT_SHIFT(EV_KEY)) & 1)) {
    return 0;
  }

  // Get the bitmask of supported keys
  if (ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(key_bits)), key_bits) < 0) {
    return 0;
  }

  // Check for a representative keyboard key (e.g., KEY_Q).
  // Mice report EV_KEY for buttons (BTN_LEFT) but not for letter keys.
  if (!((key_bits[BIT_WORD(KEY_Q)] >> BIT_SHIFT(KEY_Q)) & 1)) {
    return 0;
  }

  return 1; // It's a keyboard
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
      fds[count++] = fd;
    }
  }

  if (count == 0) {
    fprintf(stderr, "No input devices found in /dev/input/\r\n");
    return -1;
  }

  printf("Press any key on your keyboard to continue...\r\n");

  while (1) {
    fd_set readfds;
    FD_ZERO(&readfds);
    int maxfd = -1;

    for (int i = 0; i < count; ++i) {
      FD_SET(fds[i], &readfds);
      if (fds[i] > maxfd)
        maxfd = fds[i];
    }

    if (select(maxfd + 1, &readfds, NULL, NULL, NULL) > 0) {
      for (int i = 0; i < count; ++i) {
        if (FD_ISSET(fds[i], &readfds)) {
          struct input_event ev;
          if (read(fds[i], &ev, sizeof(ev)) > 0 && ev.type == EV_KEY &&
              ev.value == 1) {
            // We have a key press, now check if it's a real keyboard
            if (is_keyboard(fds[i])) {
              // SUCCESS: It's a keyboard
              printf("Keyboard detected on /dev/input/event%d\r\n", i);
              // Close all other fds
              for (int j = 0; j < count; ++j) {
                if (j != i)
                  close(fds[j]);
              }
              return fds[i]; // Return the correct fd
            } else {
              // NOT A KEYBOARD: Inform the user and wait for new input
              printf("Detected a non-keyboard device (like a mouse). Please "
                     "press a key on your KEYBOARD.\r\n");
            }
          }
        }
      }
    }
  }

  return -1; // Should not be reached
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
    if (DEBUG)
      printf("[DEBUG] Keyboard input closed.\r\n");
  }
}

void ZFB_ProcessKeyboard() {
  if (keyboard_fd == -1)
    return;

  struct input_event event;
  while (read(keyboard_fd, &event, sizeof(event)) > 0) {
    if (event.type == EV_KEY) {
      int code = event.code;
      int value = event.value;

      if (code < 0 || code >= KEY_MAX)
        continue;

      if (value == 1 && key_states[code] == 0) {
        // Key down
        key_states[code] = 1;
        ZFB_Event down_event = {.type = ZFB_EVENT_KEYDOWN,
                                .data.key.key_code = code};
        ZFB_PushEvent(&down_event);
      } else if (value == 0 && key_states[code] == 1) {
        // Key up
        key_states[code] = 0;
        ZFB_Event up_event = {.type = ZFB_EVENT_KEYUP,
                              .data.key.key_code = code};
        ZFB_PushEvent(&up_event);
      }

      // Ignore value == 2 (key repeat)
    }
  }
}

int ZFB_IsKeyPressed(int key) {
  if (key < 0 || key >= KEY_MAX)
    return 0;
  return key_states[key];
}
