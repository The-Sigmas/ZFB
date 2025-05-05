#include "../headers/renderer.h"

uint32_t rgbToHex(uint8_t r, uint8_t g, uint8_t b)
{
  return (r << 16) | (g << 8) | b;
}

uint32_t rgbToBgr(uint32_t color)
{
  uint8_t b[4];
  b[3] = (color & 0xff000000) >> 24;
  b[2] = (color & 0x00ff0000) >> 16;
  b[1] = (color & 0x0000ff00) >> 8;
  b[0] = color & 0x000000ff;

  return 0x00 | (b[3] << 8) | (b[2] << 16) | (b[1] << 24);
}

#ifdef _WIN32

#include "rendererWin.h"

#else

struct fb_var_screeninfo vinfo;
#include "rendererLinux.h"

#endif

void ZFB_FreeTextures()
{
  int childrenCount = sizeof(texes) / sizeof(ZFB_Texture);
  for (int i = 0; i < childrenCount; i++)
  {
    free(texes[i]->path);
    free(texes[i]);
  }
  return;
}

ZFB_Texture* ZFB_LoadTexture(const char* texturePath)
{
  FILE *fp = fopen(texturePath, "rb");
  if (!fp) 
  {
    perror("Failed to open PNG file");
    return NULL;
  }

  png_byte header[8];
  fread(header, 1, 8, fp);
  if (png_sig_cmp(header, 0, 8)) 
  {
    fprintf(stderr, "File is not a valid PNG file\n");
    fclose(fp);
    return NULL;
  }

  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) 
  {
    fprintf(stderr, "Error creating PNG read struct\n");
    fclose(fp);
    return NULL;
  }

  png_infop info = png_create_info_struct(png);
  if (!info) 
  {
    fprintf(stderr, "Error creating PNG info struct\n");
    png_destroy_read_struct(&png, NULL, NULL);
    fclose(fp);
    return NULL;
  }

  if (setjmp(png_jmpbuf(png))) 
  {
    fprintf(stderr, "Error during PNG initialization\n");
    png_destroy_read_struct(&png, &info, NULL);
    fclose(fp);
    return NULL;
  }

  png_init_io(png, fp);
  png_set_sig_bytes(png, 8);

  png_read_info(png, info);

  int width = png_get_image_width(png, info);
  int height = png_get_image_height(png, info);
  int bit_depth = png_get_bit_depth(png, info);
  int color_type = png_get_color_type(png, info);

  if (color_type == PNG_COLOR_TYPE_PALETTE) 
  {
    png_set_palette_to_rgb(png);
  }

  if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) 
  {
    png_set_expand_gray_1_2_4_to_8(png);
  }

  if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) 
  {
    png_set_gray_to_rgb(png);
  }

  if (png_get_valid(png, info, PNG_INFO_tRNS)) 
  {
    png_set_tRNS_to_alpha(png);
  }

  if (bit_depth == 16) 
  {
    png_set_strip_16(png);
  }

  if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY) 
  {
    png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
  }

  png_read_update_info(png, info);

  ZFB_Texture* tex = malloc(sizeof(ZFB_Texture));
  tex->w = width;
  tex->h = height;
  tex->path = malloc(width * height * 4);

  png_bytep *row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
  for (int y = 0; y < height; y++) 
  {
    row_pointers[y] = (png_bytep)(tex->path + y * width * 4);
  }

  png_read_image(png, row_pointers);

  for (int i = 0; i < width * height; i++) 
  {
    uint8_t *pixel = (uint8_t *)(tex->path + i * 4);
    uint8_t temp = pixel[0];  // R
    pixel[0] = pixel[2];      // Swap R and B
    pixel[2] = temp;          // Swap B and R
  }

  // Clean up
  png_destroy_read_struct(&png, &info, NULL);
  free(row_pointers);
  fclose(fp);

  return tex;
}

void ZFB_InitFB(ZFB_Device *dev)
{
  InitFB(dev);
}
