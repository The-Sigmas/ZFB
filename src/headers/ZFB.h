#ifndef ZFB_H
#define ZFB_H

#include "renderer.h"
#include "engine.h"
#include "key_input.h"
#include "event.h"
#include "framelimiter.h"

// TODO: Windows compatibility where possible
void ZFB_SyncEntity(ZFB_Rect* rect, ZFB_Entity entity);
void ZFB_Print(const char* text);
#ifndef _WIN32
void ZFB_Exit(ZFB_Device *dev);
void ZFB_DInfo();
#endif

#endif
