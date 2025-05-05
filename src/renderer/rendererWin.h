#ifndef RENDERERWIN_H
#define RENDERERWIN_H

// We will actually just make the developer create the WinMain themselves
// which could actually provide more control on the developers side

void InitFB(ZFB_Device *dev)
{
  // TODO: if we ever support something other than WinGDI too, add checking
  dev->fb = malloc(dev->width * dev->height * sizeof(uint32_t));

  return;
}

void ZFB_DrawRect(ZFB_Device dev, ZFB_Rect rect, ZFB_Color* color)
{
  int x, y;

  if(rect.texture != NULL)
  {
    for (y = rect.y; y < rect.y + rect.h; y++)
    {
      if (y >= dev.height || y < 0) continue;
      for (x = rect.x; x < rect.x + rect.w; x++)
      {
        if (x >= dev.width || x < 0) continue;
        int texX = ((x - rect.x) * rect.texture->w) / rect.w;
        int texY = ((y - rect.y) * rect.texture->h) / rect.h;

        uint32_t texColor = *(uint32_t *)(rect.texture->path + (texY * rect.texture->w + texX) * 4);
        
        long location = x + (y * dev.width);
        
        uint8_t *pixel = (uint8_t *)&texColor;
        uint8_t alpha = pixel[3];

        if (alpha == 255)
        {
          *((uint32_t)&dev.fb[location]) = texColor;
        } else if (alpha > 0)
        {
          uint32_t bgColor = *(uint32_t *)(dev.fb[location]);
          uint8_t *bgPixel = (uint8_t *)&bgColor;

          uint8_t outR = (pixel[0] * alpha + bgPixel[0] * (255 - alpha)) / 255;
          uint8_t outG = (pixel[1] * alpha + bgPixel[1] * (255 - alpha)) / 255;
          uint8_t outB = (pixel[2] * alpha + bgPixel[2] * (255 - alpha)) / 255;

          *(uint32_t *)(dev.fb[location]) = (outR << 16) | (outG << 8) | outB;
        }
      }
    }
  } else
  {
    for (y = rect.y; y < rect.y + rect.h; y++)
    {
      if (y >= dev.height || y < 0) continue;
      for (x = rect.x; x < rect.x + rect.w; x++)
      {
        if (x >=dev.width || x < 0) continue;
        
        long location = x + (y * dev.width);

        if (color != NULL)
        {
          *(uint32_t *)(dev.fb[location]) = rgbToHex(color->r, color->g, color->b);
        } else
        {
          *(uint32_t *)(dev.fb[location]) = 0x000000;
        }
      }
    }
  }
  return;
}

void ZFB_DrawBG(ZFB_Device dev, ZFB_Color* color, ZFB_Texture* tex)
{
  return;
}

void ZFB_Present(ZFB_Device dev)
{
  HDC hdc = GetDC(dev.hwnd);
  SetDIBitsToDevice
    (
      hdc, 0, 0,
      dev.width, dev.height,
      0, 0, 0, dev.height,
      dev.fb, (BITMAPINFO*)&dev->bmi,
      DIB_RGB_COLORS
    );
  ReleaseDC(dev.hwnd, hdc);
  return;
}

void ZFB_CreateWindow
(
  ZFB_Device *dev,
  HINSTANCE hInstance, 
  HINSTANCE hPrevInstance,
  LPSTR lpCmdLine, 
  int nShowCmd
)
{
  WNDCLASS wc =
  {
    .lpfnWndProc = WindowProc,
    .hInstance = hInstance,
    .lpszClassName = "ZFB_Window",
  };
  RegisterClass(&wc);

  // Now we get to the real window creation
  HWND hwnd = CreateWindow(
      "ZFB_Window",
      "Project",
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT,
      dev.width, dev.height,
      0, 0,
      hInstance, 0
      );
  ShowWindow(hwnd, SW_SHOW);
  dev->hwnd = hwnd;

  BITMAPINFO bmi = 
  {
    .bmiHeader.biSize = sizeof(BITMAPINFOHEADER),
    .bmiHeader.biWidth = *dev->width,
    .bmiHeader.biHeight = *dev->height,
    .bmiHeader.biPlanes = 1,
    .bmiHeader.biBitCount = 32, // Scary Larry in case of no 32bit depth
    .bmiHeader.biCompression = BI_RGB // Because who doesn't use that?
  };
  dev->bmi = bmi;

  return;
}

#endif
