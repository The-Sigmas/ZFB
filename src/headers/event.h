#ifndef ZFB_EVENT_H
#define ZFB_EVENT_H

#include<stdint.h>

typedef enum {
    ZFB_EVENT_NONE = 0,
    ZFB_EVENT_KEYDOWN,
    ZFB_EVENT_KEYUP,
    ZFB_EVENT_QUIT
} ZFB_EventType;

typedef struct {
    ZFB_EventType type;
    union {
        struct { int key_code; } key;
    };
} ZFB_Event;

typedef void (*ZFB_EventCallback)(const ZFB_Event* event);

void ZFB_Event_Init(void);
void ZFB_Event_Register(ZFB_EventType type, ZFB_EventCallback callback);
void ZFB_Event_Emit(const ZFB_Event* event);
void ZFB_Event_Shutdown(void);

#endif