#pragma once
#ifndef __cplusplus
#include <pl32-types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#else
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#endif

void plChangeAllocLimit(size_t bytes);
size_t plGetAllocSize();

void* plSafeMalloc(size_t size);
void* plSafeCalloc(size_t amount, size_t size);
void* plSafeRealloc(void* pointer, size_t size);
void plSafeFree(void* pointer);
void plSafeFreeAll();

int plAddToMemoryBuffer(plmembuf_t* array, void* pointer);
int plRemoveFromMemoryBuffer(plmembuf_t* array, void* pointer);

