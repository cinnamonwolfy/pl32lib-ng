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
typedef struct plpointer plpointer_t;
typedef struct plarray plarray_t;
typedef struct plgc plgc_t;

void plChangeAllocLimit(size_t bytes);
size_t plGetAllocSize();

void* plSafeMalloc(size_t size);
void* plSafeCalloc(size_t amount, size_t size);
void* plSafeRealloc(void* pointer, size_t size);
void plSafeFree(void* pointer);
void plSafeFreeAll();

int plAddToMemoryBuffer(plmembuf_t* array, void* pointer);
int plRemoveFromMemoryBuffer(plmembuf_t* array, void* pointer);

