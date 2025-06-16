#include "headers/ZFB.h"

// See `headers/ZFB.h` for more information
#ifdef _WIN32
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_DESTROY) PostQuitMessage(0);
  switch(msg)
  {
    case WM_DESTROY:
      {
        PostQuitMessage(0);
      }
    case WM_SIZE:
      {
        // TODO: Send resize event
      }
  }
	return DefWindowProc(hwnd, msg, wParam, lParam);
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
  if(!dev->title)
  {
    dev->title = "ZFB_Window";
  }
  WNDCLASS wc =
  {
    .lpfnWndProc = WindowProc,
    .hInstance = hInstance,
    .lpszClassName = dev->title,
  };
  RegisterClass(&wc);

  // Now we get to the real window creation
  HWND hwnd = CreateWindow(
      dev->title,
      dev->title,
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

void ZFB_WinMessage()
{
  while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  if (msg.message == WM_QUIT)
  {
    ZFB_Event e = {ZFB_EVENT_QUIT};
    ZFB_PushEvent(&e);
  }
}
#else
void ZFB_Exit(ZFB_Device *dev)
{
  munmap(dev->fbp, dev->screensize);
  close(dev->fb);
}

double get_memory_usage(long *total_out)
{
  long total, free, available;
  FILE *fp = fopen("/proc/meminfo", "r");
  if (!fp) return -1;

  fscanf(fp, "MemTotal: %ld kB\nMemFree: %ld kB\nMemAvailable: %ld kB\n",
         &total, &free, &available);
  fclose(fp);

  if (total_out) *total_out = total; // Store total memory for later use

  return 100.0 * (1 - (double)available / total);
}

long get_process_memory_usage()
{
  FILE *fp = fopen("/proc/self/status", "r");
  if (!fp) return -1;

  char line[256];
  long rss = 0;
  while (fgets(line, sizeof(line), fp))
  {
    if (sscanf(line, "VmRSS: %ld kB", &rss) == 1)
    {
      fclose(fp);
      return rss;
    }
  }

  fclose(fp);
  return -1;
}

void ZFB_DInfo()
{
  long total_memory;
  double mem_usage = get_memory_usage(&total_memory);
  long process_mem_usage = get_process_memory_usage();

  if (mem_usage < 0 || process_mem_usage < 0)
  {
    printf("Error retrieving memory usage\n");
    return;
  }

  double total_mem_mb = process_mem_usage / 1024.0;
  double process_mem_percentage = 100.0 * process_mem_usage / total_memory;

  printf("\rMemory: %.2f%% by System\r\nProcess: %.2fMB (%.2f%% of system memory)",
         mem_usage, total_mem_mb, process_mem_percentage);
  fflush(stdout);
}
#endif
void ZFB_Print(const char* text)
{
  printf("\r%s\n", text);
  return;
}

void ZFB_SyncEntity(ZFB_Rect* rect, ZFB_Entity entity)
{
  ZFB_Rect tmp =
  {
    .position = entity.physics.position,
    .width = entity.width,
    .height = entity.height,
    .rotation = entity.physics.rotation,
    .texture = rect->texture
  };
  memset(rect, 0, sizeof(rect));
  rect->position = tmp.position;
  rect->width = tmp.width;
  rect->height = tmp.height;
  rect->rotation = tmp.rotation;
  rect->texture = tmp.texture;
  return;
}
