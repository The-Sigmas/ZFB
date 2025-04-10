#ifndef RENDERERWIN_H
#define RENDERERWIN_H

static ZFB_WinRect rects[1024];
static int rect_count = 0;

void ZFB_DrawBG(ZFB_Device dev, ZFB_Color* color, ZFB_Texture* tex)
{
  return;
}

void ZFB_DrawRect(ZFB_Device dev, ZFB_Rect rect, ZFB_Color* color)
{
  return;
}

ZFB_Texture* ZFB_LoadTexture(const char* texturePath)
{
  ZFB_Texture* tex;
  return tex;
}

void InitFB(ZFB_Device *dev)
{
  
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
