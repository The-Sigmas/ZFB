#include "event.h"
#include <stdlib.h>
#include <stdio.h>

#define MAX_LISTENERS 16 

typedef struct {
    ZFB_EventType type;
    ZFB_EventCallback callbacks[MAX_LISTENERS];
    int callback_count;
} ZFB_EventListener;

static ZFB_EventListener event_listeners[ZFB_EVENT_QUIT + 1];  // Keeps listener storage internal

void ZFB_Event_Init(void) {
    int event_count = sizeof(event_listeners) / sizeof(event_listeners[0]);
    for (int i = 0; i < event_count; i++) {
        event_listeners[i].type = (ZFB_EventType)i;
        event_listeners[i].callback_count = 0;
    }
}

void ZFB_Event_Register(ZFB_EventType type, ZFB_EventCallback callback) {
    int event_count = sizeof(event_listeners) / sizeof(event_listeners[0]);
    if ((int)type >= event_count) return;  // Ensure event type is valid

    ZFB_EventListener* listener = &event_listeners[type];

    if (listener->callback_count < MAX_LISTENERS) {
        listener->callbacks[listener->callback_count++] = callback;
    } else {
        fprintf(stderr, "Warning: Too many listeners for event %d\n", type);
    }
}

void ZFB_Event_Emit(const ZFB_Event* event) {
    int event_count = sizeof(event_listeners) / sizeof(event_listeners[0]);
    if (!event || (int)event->type >= event_count) {
        fprintf(stderr, "Warning: Attempted to emit an invalid event.\n");
        return;
    }

    ZFB_EventListener* listener = &event_listeners[event->type];

    for (int i = 0; i < listener->callback_count; i++) {
        listener->callbacks[i](event);
    }
}

void ZFB_Event_Shutdown(void) {
    // No dynamic memory used, but this function is here for consistency
    int event_count = sizeof(event_listeners) / sizeof(event_listeners[0]);
    for (int i = 0; i < event_count; i++) {
        event_listeners[i].callback_count = 0;
    }
}
