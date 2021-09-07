#pragma once
#ifdef __cplusplus
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#else
#include <cstdio>
#include <cstdlib>
#endif

void changeAllocLimit(unsigned bytes);

void* safe_malloc(size_t size);
void* safe_calloc(size_t amount, size_t size);
void* safe_realloc(void* pointer, size_t size);
void safe_free(void* pointer);
void safe_free_all();

int* int_smalloc(size_t amount);
int* int_srealloc(int* pointer, size_t amount);
char* char_smalloc(size_t amount);
char* char_srealloc(char* pointer, size_t amount);

char** parse(char* input, char* delimiter);
