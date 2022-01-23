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
#else
#include <cstdlib>
#include <cerrno>
#include <cstring>

extern "C" {
#endif

#define PLGC_INIT 1
#define PLGC_CLEAN 2
#define PLGC_STOP 3

typedef struct plgc plgc_t;

int plGCManage(plgc_t* gc, int mode, void* ptr);

plmembuf_t* plGCMalloc(size_t size);
plmembuf_t* plGCCalloc(size_t amount, size_t size);
plmembuf_t* plGCRealloc(plmembuf_t* membuf, size_t size);
void plGCFree(void* pointer);

#ifdef __cplusplus
}
#endif
