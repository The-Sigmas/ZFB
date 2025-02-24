#if RENDERER_H
#define RENDERER_H

#include <string.h>
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

#endif
