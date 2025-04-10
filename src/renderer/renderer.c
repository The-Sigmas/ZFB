#include "../headers/renderer.h"

uint32_t rgbToHex(uint8_t r, uint8_t g, uint8_t b) {
  return (r << 16) | (g << 8) | b;
}

#ifdef _WIN32

#include "rendererWin.h"

#else

struct fb_var_screeninfo vinfo;
#include "rendererLinux.h"

#endif

void ZFB_InitFB(ZFB_Device *dev)
{
  InitFB(dev);
}
