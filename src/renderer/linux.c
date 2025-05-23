#include "../headers/renderer.h"

struct fb_var_screeninfo vinfo;

void ZFB_DrawBG(ZFB_Device dev, ZFB_Color* color, ZFB_Texture* tex)
{
  ZFB_Rect r = 
  { 
    {0, 0}, 
    vinfo.xres_virtual, vinfo.yres_virtual, 
    .texture = tex 
  };
  if (tex != NULL)
  {
    ZFB_DrawRect(dev, r, NULL);
  } else 
  {
    ZFB_DrawRect(dev, r, color);
  }
}

void ZFB_DrawUnrotatedRect(ZFB_Device dev, ZFB_Rect rect, ZFB_Color* color)
{
  int x, y;

  if (rect.texture != NULL) 
  {
    for (y = rect.position.y; y < rect.position.y + rect.height; y++) 
    {
      // TODO: For performance reasons, switch to the raw vinfo.[y/x]res instead of using the virtual
      if (y >= vinfo.yres_virtual || y < 0) continue;
      for (x = rect.position.x; x < rect.position.x + rect.width; x++) 
      {
        if (x >= vinfo.xres_virtual || x < 0) continue;
        int texX = ((x - rect.position.x) * rect.texture->width) / rect.width;
        int texY = ((y - rect.position.y) * rect.texture->height) / rect.height;

        uint32_t texColor = *(uint32_t *)(rect.texture->path + (texY * rect.texture->width + texX) * 4);

        long location = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) 
                      + (y + vinfo.yoffset) * vinfo.xres_virtual * (vinfo.bits_per_pixel / 8);

        uint8_t *pixel = (uint8_t *)&texColor;
        uint8_t alpha = pixel[3];

        if (alpha == 255)
        {
          *(uint32_t *)(dev.fbp + location) = texColor;
        } else if (alpha > 0)
        {
          uint32_t bgColor = *(uint32_t *)(dev.fbp + location);
          uint8_t *bgPixel = (uint8_t *)&bgColor;

          uint8_t outR = (pixel[0] * alpha + bgPixel[0] * (255 - alpha)) / 255;
          uint8_t outG = (pixel[1] * alpha + bgPixel[1] * (255 - alpha)) / 255;
          uint8_t outB = (pixel[2] * alpha + bgPixel[2] * (255 - alpha)) / 255;

          *(uint32_t *)(dev.fbp + location) = (outR << 16) | (outG << 8) | outB;
        }

        // Non alpha
        //*(uint32_t *)(dev.fbp + location) = texColor;
      }
    }
  } else
  {
    for (y = rect.position.y; y < rect.position.y + rect.height; y++) 
    {
      if (y >= vinfo.yres_virtual || y < 0) continue;
      for (x = rect.position.x; x < rect.position.x + rect.width; x++) 
      {
        if (x >= vinfo.xres_virtual || x < 0) continue;
        long location = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) 
                      + (y + vinfo.yoffset) * vinfo.xres_virtual * (vinfo.bits_per_pixel / 8);

        if (color != NULL)
        {
          *(uint32_t *)(dev.fbp + location) = rgbToHex(color->r, color->g, color->b);
        } else
        {
          *(uint32_t *)(dev.fbp + location) = 0x000000;
        }
      }
    }
  }
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

  for (int y = minY; y <= maxY; y++) {
    if (y < 0 || y >= vinfo.yres_virtual) continue;

    for (int x = minX; x <= maxX; x++) {
      if (x < 0 || x >= vinfo.xres_virtual) continue;

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

      long location = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8)
                    + (y + vinfo.yoffset) * vinfo.xres_virtual * (vinfo.bits_per_pixel / 8);

      if (alpha == 255) {
        *(uint32_t *)(dev.fbp + location) = texColor;
      } else if (alpha > 0) {
        uint32_t bgColor = *(uint32_t *)(dev.fbp + location);
        uint8_t *bgPixel = (uint8_t *)&bgColor;

        uint8_t outR = (pixel[0] * alpha + bgPixel[0] * (255 - alpha)) / 255;
        uint8_t outG = (pixel[1] * alpha + bgPixel[1] * (255 - alpha)) / 255;
        uint8_t outB = (pixel[2] * alpha + bgPixel[2] * (255 - alpha)) / 255;

        *(uint32_t *)(dev.fbp + location) = (outR << 16) | (outG << 8) | outB;
      }
    }
  }
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

void ZFB_InitFB(ZFB_Device *dev)
{
  // TODO: Check whether it's DRM or raw FB
  // NOTE: DRM is not developed yet

  dev->fb = open(dev->path, O_RDWR);
  if (dev->fb == -1) {
    perror("Error opening framebuffer device");
    return;
  }
  if (ioctl(dev->fb, FBIOGET_VSCREENINFO, &vinfo)) {
    perror("Error reading variable information");
    return;
  }

  //Thanks to this the user can read the screen properties
  dev->width = vinfo.xres;
  dev->height = vinfo.yres;

  dev->screensize = vinfo.yres_virtual * vinfo.xres_virtual * (vinfo.bits_per_pixel / 8);
  dev->fbp = (uint8_t *)mmap(0, dev->screensize, PROT_READ | PROT_WRITE, MAP_SHARED, dev->fb, 0);
  
  if ((intptr_t)dev->fbp == -1) {
    perror("Error mapping framebuffer device to memory");
    return;
  }
  return;
}
