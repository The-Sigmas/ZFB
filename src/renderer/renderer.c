#include "../headers/renderer.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include "../headers/engine.h"

struct fb_var_screeninfo vinfo;

unsigned long createRGB(int r, int g, int b)
{   
  return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
}

void InitWindowsFB()
{
  printf("Not Implemented yet\n");
  return;
}

void InitLinuxFB(ZFB_Device *dev)
{
  dev->fb = open(dev->path, O_RDWR);
  if (dev->fb == -1) {
    perror("Error opening framebuffer device");
    return;
  }
  if (ioctl(dev->fb, FBIOGET_VSCREENINFO, &vinfo)) {
    perror("Error reading variable information");
    return;
  }
  printf("Resolution: %dx%d, Bits Per Pixel: %d\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
  dev->screensize = vinfo.yres_virtual * vinfo.xres_virtual * (vinfo.bits_per_pixel / 8);
  dev->fbp = (uint8_t *)mmap(0, dev->screensize, PROT_READ | PROT_WRITE, MAP_SHARED, dev->fb, 0);
  if ((intptr_t)dev->fbp == -1) {
    perror("Error mapping framebuffer device to memory");
    return;
  }
  return;
}

void ZFB_InitFB(ZFB_Device *dev)
{
  if (strcmp(dev->path, "win") == 0)
  {
    InitWindowsFB();
  } else
  {
    InitLinuxFB(dev);
  }
}

void ZFB_DrawRect(ZFB_Device dev, ZFB_Rect rect, ZFB_Color color)
{
  int x, y;
  for (y = rect.y; y < rect.h; y++)
  {
    for (x = rect.x; x < rect.w; x++)
    {
      long location = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) + (y + vinfo.yoffset) * vinfo.xres_virtual * (vinfo.bits_per_pixel / 8);
      //if (vinfo.bits_per_pixel == 32) {
      *(uint32_t *)(dev.fbp + location) = createRGB(color.r, color.g, color.b);
      //} else if (vinfo.bits_per_pixel == 16) {
      //  *(uint16_t *)(dev.fbp + location) = (31 << 11);
      //}
    }
  }
  return;
}

void ZFB_Exit(ZFB_Device *dev)
{
  munmap(dev->fbp, dev->screensize);
  close(dev->fb);
}
