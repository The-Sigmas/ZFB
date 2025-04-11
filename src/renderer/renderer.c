#include "../headers/renderer.h"

uint32_t rgbToHex(uint8_t r, uint8_t g, uint8_t b)
{
  return (r << 16) | (g << 8) | b;
}

uint32_t rgbToBgr(uint32_t color)
{
  uint8_t b[4];
  b[3] = (color & 0xff000000) >> 24;
  b[2] = (color & 0x00ff0000) >> 16;
  b[1] = (color & 0x0000ff00) >> 8;
  b[0] = color & 0x000000ff;

  return 0x00 | (b[3] << 8) | (b[2] << 16) | (b[1] << 24);
}

COLORREF colorcompat(ZFB_Color color)
{
  COLORREF winColor;
  
  return
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
