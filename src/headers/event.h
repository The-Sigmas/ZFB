#ifndef ZFB_EVENT_H
#define ZFB_EVENT_H

#include<stdint.h>

#define MAX_EVENT_QUEUE 256

typedef enum {
    ZFB_EVENT_NONE = 0,
    ZFB_EVENT_KEYDOWN,
    ZFB_EVENT_KEYUP,
    ZFB_EVENT_QUIT,
} ZFB_EventType;

typedef struct {
    ZFB_EventType type;
    union {
        struct { int key_code; } key;
    } data;
} ZFB_Event;

typedef void (*ZFB_EventCallback)(const ZFB_Event* event);

void ZFB_Event_Init(void);
int ZFB_PushEvent(const ZFB_Event* event);
int ZFB_PollEvent(ZFB_Event* event);
void ZFB_Event_Shutdown(void);

#endif