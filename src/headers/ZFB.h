#ifndef ZFB_H
#define ZFB_H

#include "renderer.h"
#include "engine.h"
#include "key_input.h"
#include "event.h"

#ifndef _WIN32
void ZFB_Exit(ZFB_Device *dev);
void ZFB_Print(const char* text);
void ZFB_DInfo();
void ZFB_RawMode();
void ZFB_Exit_RawMode();
#endif

#endif
