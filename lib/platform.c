#include "platform.h"

#ifdef _WIN32
#include "win32.h"
PFNCREATEWINDOWPROC create_window = win32_create_window;
PFNAPPRUNNINGPROC app_running = win32_app_running;
PFNSWAPBUFFERSPROC swap_buffers = win32_swap_buffers;
PFNDESTROYWINDOWPROC destroy_window = win32_destroy_window;
#endif