#ifndef ZFB_H
#define ZFB_H

#include "engine.h"
#include "event.h"
#include "font.h"
#include "framelimiter.h"
#include "key_input.h"
#include "renderer.h"
#include "zfb_keys.h"

#ifdef _WIN32
extern MSG msg;
#endif

// TODO: Windows compatibility where possible
void ZFB_SyncEntity(ZFB_Rect *rect, ZFB_Entity entity);
void ZFB_Print(const char *text);
#ifdef _WIN32
void ZFB_CreateWindow(ZFB_Device *dev, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);
void ZFB_WinMessage();
#else
void ZFB_Exit(ZFB_Device *dev);
void ZFB_DInfo();
#endif

#endif
