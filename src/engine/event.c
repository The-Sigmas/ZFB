#include "event.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    ZFB_Event events [MAX_EVENT_QUEUE];
    int head;
    int tail;
    int count; 
} ZFB_EventQueue;

static ZFB_EventQueue event_queue = { .head = 0, .tail = 0, .count = 0 };

void ZFB_Event_Init(void) {
    event_queue.head = 0;
    event_queue.tail = 0;
    event_queue.count = 0;
}

int ZFB_PollEvent(ZFB_Event* event) {
    if (event_queue.count == 0) {
        return 0;
    }
    *event = event_queue.events[event_queue.head];
    event_queue.head = (event_queue.head + 1) % MAX_EVENT_QUEUE;
    event_queue.count--;
    return 1;
} 

int ZFB_PushEvent(const ZFB_Event* event) {
    if (event_queue.count >= MAX_EVENT_QUEUE) {
        fprintf(stderr, "Warning: Event queue is overflowing. Event discarded.\n");
    }
    event_queue.events[event_queue.tail] = *event;
    event_queue.tail = (event_queue.tail + 1) % MAX_EVENT_QUEUE;
    event_queue.count++;
    return 1;
}

void ZFB_Event_Shutdown(void) {
    event_queue.head = 0;
    event_queue.tail = 0;
    event_queue.count = 0; 
}