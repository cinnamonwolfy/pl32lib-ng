#pragma once
#ifndef __cplusplus
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#else
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#endif

typedef struct plmembuf plmembuf_t;
typedef struct plgc plgc_t;

typedef struct plpointer plpointer_t;
typedef struct plarray plarray_t;

void plGCSetAllocLimit(size_t bytes);
size_t plGCGetAllocLimit();

plmembuf_t* plGCMalloc(size_t size);
plmembuf_t* plGCCalloc(size_t amount, size_t size);
plmembuf_t* plGCRealloc(plmembuf_t* membuf, size_t size);
void plGCFree(void* pointer);
void plGCClear();

plpointer_t plGCFindInMembuf(plmembuf_t membuf, plpointer_t pointer);
