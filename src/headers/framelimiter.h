#ifndef FRAMELIMITER_H
#define FRAMELIMITER_H

#include <stdlib.h>
#include <stdint.h>

typedef struct
{
  uint32_t frame;
  uint32_t limit;
  void* params;
  void (*func)(void* params);
} ZFB_FrameLimiter;

extern uint32_t frameLimitCount;
extern ZFB_FrameLimiter* frameLimiters;

uint32_t ZFB_PushFrameLimiter(ZFB_FrameLimiter newFrameLimit);
void ZFB_FrameTick();

#endif //!FRAMELIMITER_H
