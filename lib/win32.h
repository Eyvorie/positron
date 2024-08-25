#ifndef WIN32_WINDOW_H
#define WIN32_WINDOW_H
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "platform.h"

typedef struct {
  HWND window;
  HGLRC opengl_context;
  HDC device_context;
} Win32_ctx;

Win32_ctx *win32_create_window(char *name, int width, int height);
int win32_app_running(Win32_ctx *);
void win32_swap_buffers(Win32_ctx *);
void win32_destroy_window(Win32_ctx *);
#endif