#include "ZFB.h"

// See `headers/ZFB.h` for more information
#ifndef _WIN32
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

void ZFB_Print(const char* text)
{
  printf("\r%s\n", text);
  return;
}

#endif
