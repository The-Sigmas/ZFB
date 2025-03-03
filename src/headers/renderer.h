#if RENDERER_H
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
} ZFB_Device;

typedef struct
{
	uint8_t r,g,b,a;
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

int ZFB_InitFB(ZFB_Device dev);



#endif
