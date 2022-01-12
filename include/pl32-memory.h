/********************************************\
* pl32lib, v0.01                             *
* (c)2021 pocketlinux32, Under Lesser GPLv3  *
* Memory Management/Garbage Collector module *
\********************************************/
#pragma once
#ifndef __cplusplus
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#else
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <cstdarg>
#endif

#define PLGC_INIT 0
#define PLGC_REQMEM 1
#define PLGC_REQMOREMEM 2
#define PLGC_FREEMEM 3
#define PLGC_CLEANMEM 4
#define PLGC_TERMGC 5

typedef struct plmembuf plmembuf_t;
typedef struct plgc plgc_t;
typedef struct plpointer plpointer_t;
typedef struct plarray plarray_t;

plgc_t mainGC;

int plGCGetAddress(plmembuf_t* membuf, plgc_t* gc);
int plGCManage(plgc_t* gc, int mode, ...);

plmembuf_t* plGCMalloc(size_t size);
plmembuf_t* plGCCalloc(size_t amount, size_t size);
plmembuf_t* plGCRealloc(plmembuf_t* membuf, size_t size);
void plGCFree(void* pointer);
