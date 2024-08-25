#ifndef PLATFORM_H
#define PLATFORM_H

typedef void *Window;
typedef Window* (*PFNCREATEWINDOWPROC)(char *, int, int);
typedef int (*PFNAPPRUNNINGPROC)(Window *);
typedef void (*PFNSWAPBUFFERSPROC)(Window *);
typedef void (*PFNDESTROYWINDOWPROC)(Window *);

extern PFNCREATEWINDOWPROC create_window;
extern PFNAPPRUNNINGPROC app_running;
extern PFNSWAPBUFFERSPROC swap_buffers;
extern PFNDESTROYWINDOWPROC destroy_window;
#endif