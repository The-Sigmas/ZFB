#ifndef ZFB_EVENT_H
#define ZFB_EVENT_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _WIN32
#include <unistd.h>
#endif

#define MAX_EVENT_QUEUE 256

typedef enum {
  ZFB_EVENT_NONE = 0,
  ZFB_EVENT_KEYDOWN,
  ZFB_EVENT_KEYUP,
  ZFB_EVENT_QUIT,
  ZFB_EVENT_WINDOW_RESIZE,
  ZFB_EVENT_WINDOW_MOVE,
  ZFB_EVENT_WINDOW_FOCUS,
  ZFB_EVENT_WINDOW_UNFOCUS,
  ZFB_EVENT_WINDOW_MINIMIZE,
  ZFB_EVENT_WINDOW_MAXIMIZE,
  ZFB_EVENT_WINDOW_RESTORE,
  ZFB_EVENT_WINDOW_FULLSCREEN_TOGGLE
} ZFB_EventType;

typedef enum {
  ZFB_WINDOW_FOCUS_GAINED = 1,
  ZFB_WINDOW_FOCUS_LOST = 0
} ZFB_WindowFocusState;

typedef enum {
  ZFB_FULLSCREEN_ENTER = 1,
  ZFB_FULLSCREEN_EXIT = 0
} ZFB_WindowFullscreenState;

typedef struct {
  ZFB_EventType type;
  union {
    struct {
      int key_code;
    } key;
    struct {
      int width, height;
    } resize;
    struct {
      int x, y;
    } move;
    struct {
      ZFB_WindowFocusState state;
    } focus;
    struct {
      ZFB_WindowFullscreenState state;
    } fullscreen;
  } data;
} ZFB_Event;

typedef void (*ZFB_EventCallback)(const ZFB_Event *event);

void ZFB_EventInit(void);
int ZFB_PushEvent(const ZFB_Event *event);
int ZFB_PollEvent(ZFB_Event *event);
void ZFB_EventShutdown(void);

#endif
