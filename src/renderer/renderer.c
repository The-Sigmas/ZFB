#include "../headers/renderer.h"

struct fb_var_screeninfo vinfo;

uint32_t rgbToHex(uint8_t r, uint8_t g, uint8_t b) {
  return (r << 16) | (g << 8) | b;
}
void InitWindowsFB()
{
  printf("Not Implemented yet\n");
  return;
}

void InitLinuxFB(ZFB_Device *dev)
{
  dev->fb = open(dev->path, O_RDWR);
  if (dev->fb == -1) {
    perror("Error opening framebuffer device");
    return;
  }
  if (ioctl(dev->fb, FBIOGET_VSCREENINFO, &vinfo)) {
    perror("Error reading variable information");
    return;
  }
  printf("Resolution: %dx%d, Bits Per Pixel: %d\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
  dev->screensize = vinfo.yres_virtual * vinfo.xres_virtual * (vinfo.bits_per_pixel / 8);
  dev->fbp = (uint8_t *)mmap(0, dev->screensize, PROT_READ | PROT_WRITE, MAP_SHARED, dev->fb, 0);
  if ((intptr_t)dev->fbp == -1) {
    perror("Error mapping framebuffer device to memory");
    return;
  }
  return;
}

void ZFB_InitFB(ZFB_Device *dev)
{
  if (strcmp(dev->path, "win") == 0)
  {
    InitWindowsFB();
  } else
  {
    InitLinuxFB(dev);
  }
}

void ZFB_DrawRect(ZFB_Device dev, ZFB_Rect rect, ZFB_Color color)
{
  int x, y;

  if (rect.texture != NULL) {
    int texX, texY;
    for (y = rect.y; y < rect.y + rect.h; y++) {
      for (x = rect.x; x < rect.x + rect.w; x++) {
        texX = (x - rect.x) % rect.texture->w;
        texY = (y - rect.y) % rect.texture->h;

        uint32_t texColor = *(uint32_t *)(rect.texture->path + (texY * rect.texture->w + texX) * 4);

        long location = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) + (y + vinfo.yoffset) * vinfo.xres_virtual * (vinfo.bits_per_pixel / 8);

        *(uint32_t *)(dev.fbp + location) = texColor;
      }
    }
  } else {
    for (y = rect.y; y < rect.y + rect.h; y++) {
      for (x = rect.x; x < rect.x + rect.w; x++) {
        long location = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) + (y + vinfo.yoffset) * vinfo.xres_virtual * (vinfo.bits_per_pixel / 8);
        *(uint32_t *)(dev.fbp + location) = rgbToHex(color.r, color.g, color.b);
      }
    }
  }
}

ZFB_Texture* ZFBLoadTexture(const char* texturePath)
{
  FILE *fp = fopen(texturePath, "rb");
  if (!fp) {
    perror("Failed to open PNG file");
    return NULL;
  }

  png_byte header[8];
  fread(header, 1, 8, fp);
  if (png_sig_cmp(header, 0, 8)) {
    fprintf(stderr, "File is not a valid PNG file\n");
    fclose(fp);
    return NULL;
  }

  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) {
    fprintf(stderr, "Error creating PNG read struct\n");
    fclose(fp);
    return NULL;
  }

  png_infop info = png_create_info_struct(png);
  if (!info) {
    fprintf(stderr, "Error creating PNG info struct\n");
    png_destroy_read_struct(&png, NULL, NULL);
    fclose(fp);
    return NULL;
  }

  if (setjmp(png_jmpbuf(png))) {
    fprintf(stderr, "Error during PNG initialization\n");
    png_destroy_read_struct(&png, &info, NULL);
    fclose(fp);
    return NULL;
  }

  png_init_io(png, fp);
  png_set_sig_bytes(png, 8);
  png_read_png(png, info, PNG_TRANSFORM_IDENTITY, NULL);

  int width = png_get_image_width(png, info);
  int height = png_get_image_height(png, info);
  int bit_depth = png_get_bit_depth(png, info);
  int color_type = png_get_color_type(png, info);

  if (bit_depth != 8 || color_type != PNG_COLOR_TYPE_RGBA) {
    fprintf(stderr, "Unsupported PNG format\n");
    png_destroy_read_struct(&png, &info, NULL);
    fclose(fp);
    return NULL;
  }

  ZFB_Texture* tex = malloc(sizeof(ZFB_Texture));
  tex->w = width;
  tex->h = height;
  tex->path = malloc(width * height * 4);

  png_bytep *row_pointers = png_get_rows(png, info);
  for (int y = 0; y < height; y++) {
    memcpy(tex->path + y * width * 4, row_pointers[y], width * 4);
  }

  png_destroy_read_struct(&png, &info, NULL);
  fclose(fp);

  return tex;
}

void ZFB_Exit(ZFB_Device *dev)
{
  munmap(dev->fbp, dev->screensize);
  close(dev->fb);
}
