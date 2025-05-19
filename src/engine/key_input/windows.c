
#include "../headers/key_input.h"
#include "../headers/event.h"

/* Windows implementation */

void ZFB_InitInput(void)
{
 /* no need to init because we use a window in Windows */
}

void ZFB_CloseInput(void)
{
    /* no need to close anything */
}

void ZFB_ProcessKeyboard(void)
{
    static int key_states[256] = {0};

    for (int key = 9; key < 256; ++key) {
        if (GetAsyncKeyState(key) & 0x8000) {          /* key down */
            if (!key_states[key]) {                    /* first press */
                ZFB_Event ev = {
                    .type            = ZFB_EVENT_KEYDOWN,
                    .data.key.key_code = key
                };
                ZFB_PushEvent(&ev);
            }
            key_states[key] = 1;
        } else if (key_states[key]) {                  /* key up */
            ZFB_Event ev = {
                .type            = ZFB_EVENT_KEYUP,
                .data.key.key_code = key
            };
            ZFB_PushEvent(&ev);
            key_states[key] = 0;
        }
    }
}

int ZFB_IsKeyPressed(int key)
{
    return (GetAsyncKeyState(key) & 0x8000) != 0;
}
