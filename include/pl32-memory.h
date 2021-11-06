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

typedef struct plpointer plpointer_t;
typedef struct plarray plarray_t;

int plAddToArray(plarray_t* array, void* pointer);
int plRemoveFromArray(plarray_t* array, void* pointer);

void plChangeAllocLimit(unsigned int buffer, size_t bytes);
size_t plGetAllocSize(unsigned int buffer);

void* plSafeMalloc(size_t size);
void* plSafeCalloc(size_t amount, size_t size);
void* plSafeRealloc(void* pointer, size_t size);
void plSafeFree(void* pointer);
void plSafeFreeAll();

int* plIntSafeMalloc(size_t amount);
int* plIntSafeRealloc(int* pointer, size_t amount);
char* plCharSafeMalloc(size_t amount);
char* plCharSafeRealloc(char* pointer, size_t amount);
