#ifndef RENDERER_H
#define RENDERER_H

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include "../headers/engine.h"

typedef struct
{
	char *path;
  int fb;
  uint8_t *fbp;
  size_t screensize;
} ZFB_Device;

typedef struct
{
	uint8_t r,g,b;
} ZFB_Color;

typedef struct
{
	char *path;
	int w, h;
} ZFB_Texture;

typedef struct
{
	int x, y;
	int w, h;
	ZFB_Texture* texture;
} ZFB_Rect;

extern struct fb_var_screeninfo vinfo;
void ZFB_InitFB(ZFB_Device *dev);
void ZFB_Exit(ZFB_Device *dev);
void ZFB_DrawRect(ZFB_Device *dev, ZFB_Rect rect, ZFB_Color color);

#endif
