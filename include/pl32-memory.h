/*************************************************\
* pl32lib, v0.05                                  *
* (c)2022 pocketlinux32, Under Lesser GPLv2.1     *
* Memory Management/Semi-Garbage Collector module *
\*************************************************/
#pragma once
#ifndef __cplusplus
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#else
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cerrno>
#include <cstring>

extern "C" {
#endif

#define PL32LIB_VERSION "0.05"
#define PLGC_STOP 1
#define PLGC_SEARCHPTR 2
#define PLGC_ADDPTR 3
#define PLGC_RMPTR 4
#define PLGC_REALLOC 5
#define PLGC_GET_USEDMEM 6
#define PLGC_GET_MAXMEM 7
#define PLGC_SET_MAXMEM 8


typedef struct plgc plgc_t;

plgc_t* plGCInit(size_t maxMemoryAlloc);
void plGCStop(plgc_t* gc);
size_t plGCMemAmnt(plgc_t* gc, int action, size_t size);

void* plGCAlloc(plgc_t* gc, size_t size);
void* plGCCalloc(plgc_t* gc, size_t amount, size_t size);
void* plGCRealloc(plgc_t* gc, void* pointer, size_t size);
void plGCFree(plgc_t* gc, void* pointer);

#ifdef __cplusplus
}
#endif
