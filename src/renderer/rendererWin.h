#ifndef RENDERERWIN_H
#define RENDERERWIN_H

void InitFB(ZFB_Device *dev)
{
  HWND hwnd = GetConsoleWindow();
  HDC hdc = GetDC(hwnd);

  RECT rect;
  GetClientRect(hwnd, &rect);
  int width = rect.right;
  int height = rect.bottom;

  BITMAPINFO bmi = {0};
  bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
  bmi.bmiHeader.biWidth = width;
  bmi.bmiHeader.biHeight = -height;
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;

  HBITMAP bmp = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&dev->fbp, NULL, 0);
  HDC memDC = CreateCompatibleDC(hdc);
  SelectObject(memDC, bmp);

  dev->screensize = width * height * 4;
  dev->fb = (int)(intptr_t)memDC;
  dev->path = (char*)(intptr_t)bmp;
}

void ZFB_DrawRect(ZFB_Device dev, ZFB_Rect rect, ZFB_Color* color)
{
  int width, height;
  HDC memDC = (HDC)(intptr_t)dev.fb;

  BITMAP bmp;
  GetObject((HBITMAP)dev.path, sizeof(BITMAP), &bmp);
  width = bmp.bmWidth;
  height = bmp.bmHeight;

  for (int y = rect.y; y < rect.y + rect.h; y++)
  {
    if (y < 0 || y >= height) continue;
    for (int x = rect.x; x < rect.x + rect.w; x++)
    {
      if (x < 0 || x >= width) continue;
      uint32_t* pixel = (uint32_t*)(dev.fbp + y * width * 4 + x * 4);

      if (rect.texture)
      {
        int tx = ((x - rect.x) * rect.texture->w) / rect.w;
        int ty = ((y - rect.y) * rect.texture->h) / rect.h;
        uint32_t texColor = *(uint32_t*)(rect.texture->path + (ty * rect.texture->w + tx) * 4);
        uint8_t* px = (uint8_t*)&texColor;
        uint8_t alpha = px[3];

        if (alpha == 255)
        {
          *pixel = texColor;
        } else if (alpha > 0)
        {
          uint8_t* dst = (uint8_t*)pixel;
          dst[0] = (px[0] * alpha + dst[0] * (255 - alpha)) / 255;
          dst[1] = (px[1] * alpha + dst[1] * (255 - alpha)) / 255;
          dst[2] = (px[2] * alpha + dst[2] * (255 - alpha)) / 255;
        }
      } else if (color)
      {
        *pixel = (color->r) | (color->g << 8) | (color->b << 16);
      }
    }
  }
}

void ZFB_DrawBG(ZFB_Device dev, ZFB_Color* color, ZFB_Texture* tex)
{
  ZFB_Rect r = {0, 0, 0, 0, 0, tex};

  BITMAP bmp;
  GetObject((HBITMAP)dev.path, sizeof(BITMAP), &bmp);
  r.w = bmp.bmWidth;
  r.h = bmp.bmHeight;

  ZFB_DrawRect(dev, r, color);
}

void ZFB_Present(ZFB_Device dev)
{
  HWND hwnd = GetConsoleWindow();
  HDC hdc = GetDC(hwnd);
  HDC memDC = (HDC)(intptr_t)dev.fb;

  BITMAP bmp;
  GetObject((HBITMAP)dev.path, sizeof(BITMAP), &bmp);
  BitBlt(hdc, 0, 0, bmp.bmWidth, bmp.bmHeight, memDC, 0, 0, SRCCOPY);
}

#endif
