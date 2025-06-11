#ifndef FONT_H
#define FONT_H

#include "engine.h"
#include "renderer.h"

#include <stdio.h>
#include <string.h>

// All printable ascii symbols if you need more change this
#define MAX_GLYPHS 96

typedef struct {
  char c;
  ZFB_Texture *image;
} ZFB_Char; // Every char has its own image

// load the font direcctory in the specified location returns either 0 when not
// found or 1 when found
bool ZFB_LoadFont(const char *fontDir);
// Draw text to our Framebuffer window
void ZFB_DrawText(ZFB_Device dev, const char *text, int x, int y);

#endif
