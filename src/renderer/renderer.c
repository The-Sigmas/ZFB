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

void ZFB_DrawBG(ZFB_Device dev, ZFB_Color* color, ZFB_Texture* tex)
{
  ZFB_Rect r = { 0, 0, vinfo.xres_virtual, vinfo.yres_virtual, tex };
  if (tex != NULL)
  {
    ZFB_DrawRect(dev, r, NULL);
  } else 
  {
    ZFB_DrawRect(dev, r, color);
  }
}

void ZFB_DrawRect(ZFB_Device dev, ZFB_Rect rect, ZFB_Color* color)
{
  int x, y;

  if (rect.texture != NULL) 
  {
    for (y = rect.y; y < rect.y + rect.h; y++) 
    {
      if (y >= vinfo.yres_virtual || y < 0) continue;
      for (x = rect.x; x < rect.x + rect.w; x++) 
      {
        if (x >= vinfo.xres_virtual || x < 0) continue;
        int texX = ((x - rect.x) * rect.texture->w) / rect.w;
        int texY = ((y - rect.y) * rect.texture->h) / rect.h;

        uint32_t texColor = *(uint32_t *)(rect.texture->path + (texY * rect.texture->w + texX) * 4);

        long location = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) 
                      + (y + vinfo.yoffset) * vinfo.xres_virtual * (vinfo.bits_per_pixel / 8);

        uint8_t *pixel = (uint8_t *)&texColor;
        uint8_t alpha = pixel[3];

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

        // Non alpha
        //*(uint32_t *)(dev.fbp + location) = texColor;
      }
    }
  } else
  {
    for (y = rect.y; y < rect.y + rect.h; y++) 
    {
      if (y >= vinfo.yres_virtual || y < 0) continue;
      for (x = rect.x; x < rect.x + rect.w; x++) 
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

void ZFB_Exit(ZFB_Device *dev)
{
  munmap(dev->fbp, dev->screensize);
  close(dev->fb);
}

double get_memory_usage(long *total_out) {
  long total, free, available;
  FILE *fp = fopen("/proc/meminfo", "r");
  if (!fp) return -1;

  fscanf(fp, "MemTotal: %ld kB\nMemFree: %ld kB\nMemAvailable: %ld kB\n",
         &total, &free, &available);
  fclose(fp);

  if (total_out) *total_out = total; // Store total memory for later use

  return 100.0 * (1 - (double)available / total);
}

long get_process_memory_usage() {
  FILE *fp = fopen("/proc/self/status", "r");
  if (!fp) return -1;

  char line[256];
  long rss = 0;
  while (fgets(line, sizeof(line), fp)) {
    if (sscanf(line, "VmRSS: %ld kB", &rss) == 1) {
      fclose(fp);
      return rss;
    }
  }

  fclose(fp);
  return -1;
}

void ZFB_DInfo() {
  long total_memory;
  double mem_usage = get_memory_usage(&total_memory);
  long process_mem_usage = get_process_memory_usage();

  if (mem_usage < 0 || process_mem_usage < 0) {
    printf("Error retrieving memory usage\n");
    return;
  }

  double total_mem_mb = process_mem_usage / 1024.0;
  double process_mem_percentage = 100.0 * process_mem_usage / total_memory;

  printf("\rMemory: %.2f%% by System\r\nProcess: %.2fMB (%.2f%% of system memory)",
         mem_usage, total_mem_mb, process_mem_percentage);
  fflush(stdout);
}

void ZFB_Print(const char* text)
{
  printf("\r%s\n", text);
  return;
}
