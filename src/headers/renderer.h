#ifndef RENDERER_H
#define RENDERER_H

#include "../headers/engine.h"
#include <string.h>
#include <png.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>

#else

#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#endif

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
	int x, y,
			w, h;
	ZFB_Texture* texture;
} ZFB_Rect;

extern struct fb_var_screeninfo vinfo;
void ZFB_InitFB(ZFB_Device *dev);
void ZFB_DrawRect(ZFB_Device dev, ZFB_Rect rect, ZFB_Color* color);
void ZFB_DrawBG(ZFB_Device dev, ZFB_Color* color, ZFB_Texture* tex);
ZFB_Texture* ZFB_LoadTexture(const char* texturePath);

#define ZFB_Red    ((ZFB_Color){255, 0, 0})
#define ZFB_Green  ((ZFB_Color){0, 255, 0})
#define ZFB_Blue   ((ZFB_Color){0, 0, 255})
#define ZFB_White  ((ZFB_Color){255, 255, 255})
#define ZFB_Black  ((ZFB_Color){0, 0, 0})
#define ZFB_Yellow ((ZFB_Color){255, 255, 0})
#define ZFB_Cyan   ((ZFB_Color){0, 255, 255})
#define ZFB_Magenta ((ZFB_Color){255, 0, 255})
#define ZFB_Gray   ((ZFB_Color){128, 128, 128})
#define ZFB_Orange ((ZFB_Color){255, 165, 0})
#define ZFB_Purple ((ZFB_Color){128, 0, 128})
#define ZFB_Brown  ((ZFB_Color){165, 42, 42})

#endif
