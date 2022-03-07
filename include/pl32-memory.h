/*************************************************\
* pl32lib, v0.01                                  *
* (c)2021 pocketlinux32, Under Lesser GPLv3       *
* Memory Management/Semi-Garbage Collector module *
\*************************************************/
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
#define PLGC_STOP 2
#define PLGC_SEARCHPTR 3
#define PLGC_ADDPTR 4
#define PLGC_RMPTR 5
#define PLGC_MODPTR 6
#define PLGC_GET_USEDMEM 7
#define PLGC_SET_MAXMEM 8

typedef struct plgc plgc_t;

plgc_t* plGCInit(size_t maxMemoryAlloc);
int plGCManage(plgc_t* gc, int mode, void* ptr, size_t size);
size_t plGCMemAmnt(plgc_t* gc, int action, size_t size);

void* plGCAlloc(plgc_t* gc, size_t size);
void* plGCCalloc(plgc_t* gc, size_t amount, size_t size);
void* plGCRealloc(plgc_t* gc, void* pointer, size_t size);
void plGCFree(plgc_t* gc, void* pointer);

#ifdef __cplusplus
}
#endif
