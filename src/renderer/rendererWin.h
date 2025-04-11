#ifndef RENDERERWIN_H
#define RENDERERWIN_H

#define CLASS_NAME "ZFBWindow"

static ZFB_WinRect rects[1024];
static int rect_count = 0;
static HWND hwnd = NULL;
static HINSTANCE hInst = NULL;

void ZFB_DrawBG(ZFB_Device dev, ZFB_Color* color, ZFB_Texture* tex)
{
  return;
}

void ZFB_DrawRect(ZFB_Device dev, ZFB_Rect rect, ZFB_Color* color)
{
  return;
}

void ZFB_PushRect(ZFB_Rect rect, ZFB_Color* color)
{
  rects[rect_count].id = rect.id;
  
  rects[rect_count].x = rect.x;
  rects[rect_count].y = rect.y;
  rects[rect_count].w = rect.w;
  rects[rect_count].h = rect.h;

  if (color != NULL)
  {
    rects[rect_count].color = rgbToBgr(color);
  } else
  {
    COLORREF bckColor = 0x00000000;
    rects[rect_count].color = bckColor;
  }
  return;
}

ZFB_Texture* ZFB_LoadTexture(const char* texturePath)
{
  ZFB_Texture* tex;
  return tex;
}

void InitFB(ZFB_Device *dev, HINSTANCE hInstance, int nCmdShow)
{
  hInst = hInstance;
  WNDCLASS wc = {0};
  wc.lpfnWndProc = WindowProc;
  wc.hInstace = hInstace;
  wc.lpszClassName = CLASS_NAME;
  RegisterClass(&wc);

  hwnd = CreateWindowEx
    (
     0, CLASS_NAME, "ZFB",
     WS_OVERLAPPEDWINDOW,
     CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
     NULL, NULL, hInstance, NULL
    );

  ShowWindow(hwnd, nCmdShow);

  return;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch(uMsg)
  {
    case WM_PAINT:
      {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        
        for (int i = 0; i < rect_count; i++)
        {
          HBRUSH brush = CreateSolidBrush(rects[i].color);
          HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);
          Rectangle
            (hdc,
             rects[i].x,
             rects[i].y,
             rects[i].x + rects[i].w,
             rects[i].y + rects[i].h
             );
          SelectObject(hdc, oldBrush);
          DeleteObject(brush);
        }
      }
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
#endif
