#include "../headers/renderer.h"

// We will actually just make the developer create the WinMain themselves
// which could actually provide more control on the developers side

void ZFB_InitFB(ZFB_Device *dev)
{
  // TODO: if we ever support something other than WinGDI too, add checking
  dev->fb = malloc(dev->width * dev->height * sizeof(uint32_t));

  return;
}

void ZFB_DrawRotatedRect(ZFB_Device dev, ZFB_Rect rect, ZFB_Color* color)
{
  float sinR = sinf(-rect.rotation);
  float cosR = cosf(-rect.rotation);

  int halfW = rect.width / 2;
  int halfH = rect.height / 2;

  int cx = rect.position.x + halfW;
  int cy = rect.position.y + halfH;

  int minX = cx - rect.width;
  int maxX = cx + rect.width;
  int minY = cy - rect.height;
  int maxY = cy + rect.height;

  for (int y = minY; y <= maxY; y++)
  {
    if (y < 0 || y >= dev.height) continue;

    for (int x = minX; x <= maxX; x++)
    {
      if (x < 0 || x >= dev.width) continue;

      float dx = x - cx;
      float dy = y - cy;

      float localX = dx * cosR - dy * sinR + halfW;
      float localY = dx * sinR + dy * cosR + halfH;

      if (localX < 0 || localX >= rect.width || localY < 0 || localY >= rect.height)
        continue;

      int texX = (int)(localX * rect.texture->width / rect.width);
      int texY = (int)(localY * rect.texture->height / rect.height);

      if (texX < 0 || texX >= rect.texture->width || texY < 0 || texY >= rect.texture->height)
        continue;

      uint32_t texColor = *(uint32_t *)(rect.texture->path + (texY * rect.texture->width + texX) * 4);
      uint8_t *pixel = (uint8_t *)&texColor;
      uint8_t alpha = pixel[3];

      uint32_t *fbPixel = dev.fb + (y * dev.width + x);

      if (alpha == 255)
      {
        *fbPixel = texColor;
      } else if (alpha > 0)
      {
        uint32_t bgColor = *fbPixel;
        uint8_t *bgPixel = (uint8_t *)&bgColor;

        uint8_t outR = (pixel[0] * alpha + bgPixel[0] * (255 - alpha)) / 255;
        uint8_t outG = (pixel[1] * alpha + bgPixel[1] * (255 - alpha)) / 255;
        uint8_t outB = (pixel[2] * alpha + bgPixel[2] * (255 - alpha)) / 255;

        *fbPixel = (outR) | (outG << 8) | (outB << 16); // BGRA assumed
      }
    }
  }
}

void ZFB_DrawUnrotatedRect(ZFB_Device dev, ZFB_Rect rect, ZFB_Color* color)
{
  int x, y;

  if(rect.texture != NULL)
  {
    for (y = rect.position.y; y < rect.position.y + rect.height; y++)
    {
      if (y >= dev.height || y < 0) continue;
      for (x = rect.position.x; x < rect.position.x + rect.width; x++)
      {
        if (x >= dev.width || x < 0) continue;
        int texX = ((x - rect.position.x) * rect.texture->width) / rect.width;
        int texY = ((y - rect.position.y) * rect.texture->height) / rect.height;

        uint32_t texColor = *(uint32_t *)(rect.texture->path + (texY * rect.texture->width + texX) * 4);
        
        long location = x + (y * dev.width);
        
        uint8_t *pixel = (uint8_t *)&texColor;
        uint8_t alpha = pixel[3];

        if (alpha == 255)
        {
          ((uint32_t *)dev.fb)[location] = texColor;
        } else if (alpha > 0)
        {
          uint32_t bgColor = *(uint32_t *)(dev.fb[location]);
          uint8_t *bgPixel = (uint8_t *)&bgColor;

          uint8_t outR = (pixel[0] * alpha + bgPixel[0] * (255 - alpha)) / 255;
          uint8_t outG = (pixel[1] * alpha + bgPixel[1] * (255 - alpha)) / 255;
          uint8_t outB = (pixel[2] * alpha + bgPixel[2] * (255 - alpha)) / 255;

          ((uint32_t *)dev.fb)[location] = (outR << 16) | (outG << 8) | outB;
        }
      }
    }
  } else
  {
    for (y = rect.position.y; y < rect.position.y + rect.height; y++)
    {
      if (y >= dev.height || y < 0) continue;
      for (x = rect.position.x; x < rect.position.x + rect.width; x++)
      {
        if (x >=dev.width || x < 0) continue;
        
        long location = x + (y * dev.width);

        if (color != NULL)
        {
          ((uint32_t *)dev.fb)[location] = rgbToHex(color->r, color->g, color->b);
        } else
        {
          ((uint32_t *)dev.fb)[location] = 0x000000;
        }
      }
    }
  }
  return;
}

void ZFB_DrawRect(ZFB_Device dev, ZFB_Rect rect, ZFB_Color* color)
{
  if (rect.rotation == 0.0f)
  {
    ZFB_DrawUnrotatedRect(dev, rect, color);
  }
  else
  {
    ZFB_DrawRotatedRect(dev, rect, color);
  }
}

void ZFB_DrawBG(ZFB_Device dev, ZFB_Color* color, ZFB_Texture* tex)
{
  ZFB_Rect r =
  {
    {0, 0},
    dev.width, dev.height,
    .texture = tex
  };
  if(tex != NULL)
  {
    ZFB_DrawRect(dev, r, NULL);
  } else
  {
    ZFB_DrawRect(dev, r, color);
  }
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
      dev.fb, (BITMAPINFO*)&dev.bmi,
      DIB_RGB_COLORS
    );
  ReleaseDC(dev.hwnd, hdc);
  return;
}

// Dirty little cheat to get cmake to build it
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
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
      "ZFB_Window",
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT,
      dev->width, dev->height,
      0, 0,
      hInstance, 0
      );
  ShowWindow(hwnd, SW_SHOW);
  dev->hwnd = hwnd;

  BITMAPINFO bmi =
  {
    .bmiHeader.biSize = sizeof(BITMAPINFOHEADER),
    .bmiHeader.biWidth = dev->width,
    .bmiHeader.biHeight = dev->height * (-1), // We flip the biHeight so we scan top to bottom.
    .bmiHeader.biPlanes = 1,
    .bmiHeader.biBitCount = 32, // Scary Larry in case of no 32bit depth
    .bmiHeader.biCompression = BI_RGB // Because who doesn't use that?
  };
  dev->bmi = bmi;

  return;
}
