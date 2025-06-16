#ifndef RENDERER_H
#define RENDERER_H

// We are keeping this globally across all systems in case another system also requires a window creation
#define WIN_TITLE "ZFB_Project"

#include "../headers/engine.h"
#include "../headers/event.h"
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

#ifdef _WIN32
typedef struct
{
	uint32_t *fb;
	unsigned int width, height;
  const char *title;
	HWND hwnd; // Not sure if we should include hInstance aswell
	BITMAPINFO bmi; // Let's make the developer pass this one in aswell considering "Freedom"
} ZFB_Device;
#else
typedef struct
{
	char *path;
  int fb;
  uint8_t *fbp;
	unsigned int width, height;
  size_t screensize;
} ZFB_Device;
#endif

typedef struct
{
	uint8_t r,g,b;
} ZFB_Color;

typedef struct
{
	char *path;
	int width, height;
} ZFB_Texture;

typedef struct
{
	ZFB_Vector2 position;
	int width, height;
  float rotation;
	ZFB_Texture* texture;
} ZFB_Rect;

extern ZFB_Texture** texes;
#ifdef _WIN32
#else
extern struct fb_var_screeninfo vinfo;
#endif

void ZFB_InitFB(ZFB_Device *dev);
void ZFB_UpdateFB(ZFB_Device *dev);
void ZFB_DrawRect(ZFB_Device dev, ZFB_Rect rect, ZFB_Color* color);
void ZFB_DrawUnrotatedRect(ZFB_Device dev, ZFB_Rect rect, ZFB_Color* color);
void ZFB_DrawRotatedRect(ZFB_Device dev, ZFB_Rect rect, ZFB_Color* color);
void ZFB_DrawBG(ZFB_Device dev, ZFB_Color* color, ZFB_Texture* tex);
ZFB_Texture* ZFB_LoadTexture(const char* texturePath);
void ZFB_FreeTextures();
#ifdef _WIN32
void ZFB_Present(ZFB_Device dev);
#endif

uint32_t rgbToHex(uint8_t r, uint8_t g, uint8_t b);
uint32_t rgbToBgr(uint32_t color);

#define ZFB_Red         ((ZFB_Color){255, 0, 0})
#define ZFB_Green       ((ZFB_Color){0, 255, 0})
#define ZFB_Blue        ((ZFB_Color){0, 0, 255})
#define ZFB_Yellow      ((ZFB_Color){255, 255, 0})
#define ZFB_Orange      ((ZFB_Color){255, 165, 0})
#define ZFB_Purple      ((ZFB_Color){128, 0, 128})
#define ZFB_Pink        ((ZFB_Color){255, 192, 203})
#define ZFB_Cyan        ((ZFB_Color){0, 255, 255})
#define ZFB_Magenta     ((ZFB_Color){255, 0, 255})
#define ZFB_Teal        ((ZFB_Color){0, 128, 128})
#define ZFB_Indigo      ((ZFB_Color){75, 0, 130})
#define ZFB_Lime        ((ZFB_Color){0, 255, 0})
#define ZFB_Brown       ((ZFB_Color){165, 42, 42})
#define ZFB_Gray        ((ZFB_Color){128, 128, 128})
#define ZFB_DarkGray    ((ZFB_Color){64, 64, 64})
#define ZFB_LightGray   ((ZFB_Color){192, 192, 192})
#define ZFB_White       ((ZFB_Color){255, 255, 255})
#define ZFB_Black       ((ZFB_Color){0, 0, 0})


#endif
