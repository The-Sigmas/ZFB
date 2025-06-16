#include "../headers/font.h"

static ZFB_Char glyphs[MAX_GLYPHS];
static int glyphCount = 0;

static ZFB_Texture *FindGlyph(char c) {
  for (int i = 0; i < glyphCount; i++)
    if (glyphs[i].c == c) {
      return glyphs[i].image;
    }
  return NULL;
}

bool ZFB_LoadFont(const char *fontDir) {
  glyphCount = 0;
  for (char c = 32; c < 127; c++) {
    char path[256];
    snprintf(path, sizeof(path), "%s/%d.png", fontDir, c);
    ZFB_Texture *tex = ZFB_LoadTexture(path);
    if (!tex) {
      continue;
    }
    glyphs[glyphCount++] = (ZFB_Char){c, tex};
    if (glyphCount >= MAX_GLYPHS) {
      break;
    }
  }
  return glyphCount > 0;
}

void ZFB_DrawText(ZFB_Device dev, const char *text, int x, int y) {
  int px = x;
  for (const char *p = text; *p; p++) {
    ZFB_Texture *tex = FindGlyph(*p);
    if (!tex) {
      continue;
    }
    ZFB_Rect dst = {
        .position = {px, y},
        .width = tex->width,
        .height = tex->height,
    };
    ZFB_DrawRect(dev, dst, NULL);
    px += tex->width;
  }
}
