#ifndef RENDERERWIN_H
#define RENDERERWIN_H

void InitFB(ZFB_Device *dev)
{
  uint32_t fb[dev->width * dev->height];
  dev->fb = fb;
  return;
}

void ZFB_DrawRect(ZFB_Device dev, ZFB_Rect rect, ZFB_Color* color)
{
  return;
}

void ZFB_DrawBG(ZFB_Device dev, ZFB_Color* color, ZFB_Texture* tex)
{
  return;
}

void ZFB_Present()
{
  return;
}

#endif
