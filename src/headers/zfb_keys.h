#ifndef ZFB_KEYS_H
#define ZFB_KEYS_H

#if defined(_WIN32) /* ───────── Windows branch ───────── */

#include <windows.h>
/* Alphanumeric ----------------------------------------------------------- */
#define ZFB_Key_A 'A'
#define ZFB_Key_B 'B'
#define ZFB_Key_C 'C'
#define ZFB_Key_D 'D'
#define ZFB_Key_E 'E'
#define ZFB_Key_F 'F'
#define ZFB_Key_G 'G'
#define ZFB_Key_H 'H'
#define ZFB_Key_I 'I'
#define ZFB_Key_J 'J'
#define ZFB_Key_K 'K'
#define ZFB_Key_L 'L'
#define ZFB_Key_M 'M'
#define ZFB_Key_N 'N'
#define ZFB_Key_O 'O'
#define ZFB_Key_P 'P'
#define ZFB_Key_Q 'Q'
#define ZFB_Key_R 'R'
#define ZFB_Key_S 'S'
#define ZFB_Key_T 'T'
#define ZFB_Key_U 'U'
#define ZFB_Key_V 'V'
#define ZFB_Key_W 'W'
#define ZFB_Key_X 'X'
#define ZFB_Key_Y 'Y'
#define ZFB_Key_Z 'Z'

#define ZFB_Key_0 '0'
#define ZFB_Key_1 '1'
#define ZFB_Key_2 '2'
#define ZFB_Key_3 '3'
#define ZFB_Key_4 '4'
#define ZFB_Key_5 '5'
#define ZFB_Key_6 '6'
#define ZFB_Key_7 '7'
#define ZFB_Key_8 '8'
#define ZFB_Key_9 '9'

/* Function keys ---------------------------------------------------------- */
#define ZFB_Key_F1 VK_F1
#define ZFB_Key_F2 VK_F2
#define ZFB_Key_F3 VK_F3
#define ZFB_Key_F4 VK_F4
#define ZFB_Key_F5 VK_F5
#define ZFB_Key_F6 VK_F6
#define ZFB_Key_F7 VK_F7
#define ZFB_Key_F8 VK_F8
#define ZFB_Key_F9 VK_F9
#define ZFB_Key_F10 VK_F10
#define ZFB_Key_F11 VK_F11
#define ZFB_Key_F12 VK_F12

/* System / modifiers ----------------------------------------------------- */
#define ZFB_Key_Escape VK_ESCAPE
#define ZFB_Key_Tab VK_TAB
#define ZFB_Key_Enter VK_RETURN
#define ZFB_Key_Backspace VK_BACK
#define ZFB_Key_Space VK_SPACE
#define ZFB_Key_LeftShift VK_LSHIFT
#define ZFB_Key_RightShift VK_RSHIFT
#define ZFB_Key_LeftControl VK_LCONTROL
#define ZFB_Key_RightControl VK_RCONTROL
#define ZFB_Key_LeftAlt VK_LMENU
#define ZFB_Key_RightAlt VK_RMENU
#define ZFB_Key_LeftSuper VK_LWIN
#define ZFB_Key_RightSuper VK_RWIN

/* Navigation ------------------------------------------------------------- */
#define ZFB_Key_ArrowLeft VK_LEFT
#define ZFB_Key_ArrowRight VK_RIGHT
#define ZFB_Key_ArrowUp VK_UP
#define ZFB_Key_ArrowDown VK_DOWN
#define ZFB_Key_Insert VK_INSERT
#define ZFB_Key_Delete VK_DELETE
#define ZFB_Key_Home VK_HOME
#define ZFB_Key_End VK_END
#define ZFB_Key_PageUp VK_PRIOR
#define ZFB_Key_PageDown VK_NEXT

/* Numpad (numeric keypad) ------------------------------------------------ */
#define ZFB_Key_Numpad0 VK_NUMPAD0
#define ZFB_Key_Numpad1 VK_NUMPAD1
#define ZFB_Key_Numpad2 VK_NUMPAD2
#define ZFB_Key_Numpad3 VK_NUMPAD3
#define ZFB_Key_Numpad4 VK_NUMPAD4
#define ZFB_Key_Numpad5 VK_NUMPAD5
#define ZFB_Key_Numpad6 VK_NUMPAD6
#define ZFB_Key_Numpad7 VK_NUMPAD7
#define ZFB_Key_Numpad8 VK_NUMPAD8
#define ZFB_Key_Numpad9 VK_NUMPAD9
#define ZFB_Key_NumpadAdd VK_ADD
#define ZFB_Key_NumpadSub VK_SUBTRACT
#define ZFB_Key_NumpadMul VK_MULTIPLY
#define ZFB_Key_NumpadDiv VK_DIVIDE
#define ZFB_Key_NumpadDecimal VK_DECIMAL
#define ZFB_Key_NumpadEnter VK_RETURN /* keypad Enter is still VK_RETURN */

#elif defined(__linux__) /* ───────── Linux branch ───────── */

#include <linux/input-event-codes.h> /* brings in KEY_* */

/* Alphanumeric ----------------------------------------------------------- */
#define ZFB_Key_A KEY_A
#define ZFB_Key_B KEY_B
#define ZFB_Key_C KEY_C
#define ZFB_Key_D KEY_D
#define ZFB_Key_E KEY_E
#define ZFB_Key_F KEY_F
#define ZFB_Key_G KEY_G
#define ZFB_Key_H KEY_H
#define ZFB_Key_I KEY_I
#define ZFB_Key_J KEY_J
#define ZFB_Key_K KEY_K
#define ZFB_Key_L KEY_L
#define ZFB_Key_M KEY_M
#define ZFB_Key_N KEY_N
#define ZFB_Key_O KEY_O
#define ZFB_Key_P KEY_P
#define ZFB_Key_Q KEY_Q
#define ZFB_Key_R KEY_R
#define ZFB_Key_S KEY_S
#define ZFB_Key_T KEY_T
#define ZFB_Key_U KEY_U
#define ZFB_Key_V KEY_V
#define ZFB_Key_W KEY_W
#define ZFB_Key_X KEY_X
#define ZFB_Key_Y KEY_Y
#define ZFB_Key_Z KEY_Z

#define ZFB_Key_0 KEY_0
#define ZFB_Key_1 KEY_1
#define ZFB_Key_2 KEY_2
#define ZFB_Key_3 KEY_3
#define ZFB_Key_4 KEY_4
#define ZFB_Key_5 KEY_5
#define ZFB_Key_6 KEY_6
#define ZFB_Key_7 KEY_7
#define ZFB_Key_8 KEY_8
#define ZFB_Key_9 KEY_9

/* Function keys ---------------------------------------------------------- */
#define ZFB_Key_F1 KEY_F1
#define ZFB_Key_F2 KEY_F2
#define ZFB_Key_F3 KEY_F3
#define ZFB_Key_F4 KEY_F4
#define ZFB_Key_F5 KEY_F5
#define ZFB_Key_F6 KEY_F6
#define ZFB_Key_F7 KEY_F7
#define ZFB_Key_F8 KEY_F8
#define ZFB_Key_F9 KEY_F9
#define ZFB_Key_F10 KEY_F10
#define ZFB_Key_F11 KEY_F11
#define ZFB_Key_F12 KEY_F12

/* System / modifiers ----------------------------------------------------- */
#define ZFB_Key_Escape KEY_ESC
#define ZFB_Key_Tab KEY_TAB
#define ZFB_Key_Enter KEY_ENTER
#define ZFB_Key_Backspace KEY_BACKSPACE
#define ZFB_Key_Space KEY_SPACE
#define ZFB_Key_LeftShift KEY_LEFTSHIFT
#define ZFB_Key_RightShift KEY_RIGHTSHIFT
#define ZFB_Key_LeftControl KEY_LEFTCTRL
#define ZFB_Key_RightControl KEY_RIGHTCTRL
#define ZFB_Key_LeftAlt KEY_LEFTALT
#define ZFB_Key_RightAlt KEY_RIGHTALT
#define ZFB_Key_LeftSuper KEY_LEFTMETA
#define ZFB_Key_RightSuper KEY_RIGHTMETA

/* Navigation ------------------------------------------------------------- */
#define ZFB_Key_ArrowLeft KEY_LEFT
#define ZFB_Key_ArrowRight KEY_RIGHT
#define ZFB_Key_ArrowUp KEY_UP
#define ZFB_Key_ArrowDown KEY_DOWN
#define ZFB_Key_Insert KEY_INSERT
#define ZFB_Key_Delete KEY_DELETE
#define ZFB_Key_Home KEY_HOME
#define ZFB_Key_End KEY_END
#define ZFB_Key_PageUp KEY_PAGEUP
#define ZFB_Key_PageDown KEY_PAGEDOWN

/* Numpad ----------------------------------------------------------------- */
#define ZFB_Key_Numpad0 KEY_KP0
#define ZFB_Key_Numpad1 KEY_KP1
#define ZFB_Key_Numpad2 KEY_KP2
#define ZFB_Key_Numpad3 KEY_KP3
#define ZFB_Key_Numpad4 KEY_KP4
#define ZFB_Key_Numpad5 KEY_KP5
#define ZFB_Key_Numpad6 KEY_KP6
#define ZFB_Key_Numpad7 KEY_KP7
#define ZFB_Key_Numpad8 KEY_KP8
#define ZFB_Key_Numpad9 KEY_KP9
#define ZFB_Key_NumpadAdd KEY_KPPLUS
#define ZFB_Key_NumpadSub KEY_KPMINUS
#define ZFB_Key_NumpadMul KEY_KPASTERISK
#define ZFB_Key_NumpadDiv KEY_KPSLASH
#define ZFB_Key_NumpadDecimal KEY_KPDOT
#define ZFB_Key_NumpadEnter KEY_KPENTER

#else
#error "Unsupported platform for ZFB keys"
#endif

#endif /* ZFB_KEYS_H */
