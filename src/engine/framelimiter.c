#include "../headers/framelimiter.h"

uint32_t frameLimitCount = 0;
ZFB_FrameLimiter* frameLimiters = NULL;

uint32_t ZFB_PushFrameLimiter(ZFB_FrameLimiter newFrameLimit)
{
  frameLimitCount++;
  frameLimiters = realloc(frameLimiters, sizeof(ZFB_FrameLimiter) * frameLimitCount);
  frameLimiters[frameLimitCount=1] = newFrameLimit;
  return frameLimitCount;
}

void ZFB_FrameTick()
{
  for(int frameLimit = 0; frameLimit < frameLimitCount; frameLimit++)
  {
    if(frameLimiters[frameLimit].frame < frameLimiters[frameLimit].limit)
    {
      frameLimiters[frameLimit].frame++;
    } else
    {
      frameLimiters[frameLimit].func(frameLimiters[frameLimit].params);
      frameLimiters[frameLimit].frame = 0;
    }
  }
}
