#ifndef UNICODE
#define UNICODE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "win32.h"
#include "alloc.h"
#include "opengl.h"

LRESULT CALLBACK handle_win32_event(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam);

Win32_ctx *win32_create_window(char *name, int width, int height)
{
  // set class name
  // The maximum length for lpszClassName is 256.
  // If lpszClassName is greater than the maximum length, the RegisterClass function will fail.
  // https://learn.microsoft.com/en-au/windows/win32/api/winuser/ns-winuser-wndclassa
  size_t window_name_len = strnlen(name, 255);
  wchar_t class_name[256];
  size_t bytes_converted =
    mbstowcs(class_name, name, window_name_len + 1);
  class_name[bytes_converted] = 0;

  HINSTANCE instance =
    GetModuleHandle(NULL);
  WNDCLASSEXW class = { 0 };
  class.cbSize        = sizeof(class);
  class.lpfnWndProc   = handle_win32_event;
  class.hInstance     = instance;
  class.lpszClassName = class_name;

  ATOM class_registered =
    RegisterClassExW(&class);
  if (!class_registered) {
    printf("[Error %d]: failed to register class\n", GetLastError());
    exit(EXIT_FAILURE);
  }

  Win32_ctx *ctx = new(Win32_ctx);
  ctx->window = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
    class_name, class_name, WS_OVERLAPPEDWINDOW,
      100, 120, width, height,
        NULL, NULL, instance, NULL);
  if (!ctx->window) {
    free(ctx);
    printf("[Error %d]: failed create window\n", GetLastError());
    exit(EXIT_FAILURE);
  }

  ctx->device_context = GetDC(ctx->window);
  PIXELFORMATDESCRIPTOR desired_pixel_format = { 0 };
  desired_pixel_format.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
  desired_pixel_format.nVersion   = 1;
  desired_pixel_format.dwFlags    = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
  desired_pixel_format.cColorBits = 32;
  desired_pixel_format.cAlphaBits = 8;
  desired_pixel_format.iLayerType = PFD_MAIN_PLANE;

  int suggested_pixel_format_index = ChoosePixelFormat(ctx->device_context,
    &desired_pixel_format);
  if (!suggested_pixel_format_index) {
    free(ctx);
    printf("[Error %d]: failed choose pixel format\n", GetLastError());
    exit(EXIT_FAILURE);
  }

  PIXELFORMATDESCRIPTOR suggested_format;
  DescribePixelFormat(ctx->device_context,
    suggested_pixel_format_index, sizeof(PIXELFORMATDESCRIPTOR), &suggested_format);
  SetPixelFormat(ctx->device_context,
    suggested_pixel_format_index, &suggested_format);

  HGLRC temp_rendering_context = 
    wglCreateContext(ctx->device_context);
  wglMakeCurrent(ctx->device_context, temp_rendering_context);

  int attribs[] = {
    0x2091, 3,
    0x2092, 3,
    0x2094, 0,
    0
  };

  opengl_init();
  HGLRC opengl3_plus_rendering_context =
    wgl_create_context_attribs_arb(ctx->device_context, 0, attribs);
  wglMakeCurrent(NULL, NULL);
  wglMakeCurrent(ctx->device_context, opengl3_plus_rendering_context);
  wglDeleteContext(temp_rendering_context);

  ShowWindow(ctx->window, true);
  UpdateWindow(ctx->window);

  if (!SetProp(ctx->window, L"context", ctx))
    printf("failed to set  window prop\n");

  return ctx;
}

void win32_destroy_window(Win32_ctx *ctx)
{
  ReleaseDC(ctx->device_context, ctx->window);
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(ctx->opengl_context);
  DestroyWindow(ctx->window);
  //RemoveProp(ctx->window, "context");
}

int win32_app_running(Win32_ctx *ctx)
{
  MSG message = { 0 };
  if (PeekMessage(&message, ctx->window, 0, 0, PM_REMOVE)) {
    TranslateMessage(&message);
    DispatchMessage(&message);
  }
  return 1;
}

void win32_swap_buffers(Win32_ctx *ctx)
{
  SwapBuffers(ctx->device_context);
}

LRESULT CALLBACK handle_win32_event(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
  Win32_ctx *context = (Win32_ctx *)GetProp(window, L"context");
  switch (message) {
    case WM_DESTROY: {
      RemoveProp(window, L"context");
      PostQuitMessage(0);
      return 0;
    }
  }

  return DefWindowProc(window, message, wparam, lparam);
}