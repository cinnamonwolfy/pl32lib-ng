#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void changeAllocLimit(unsigned bytes);

void* safe_malloc(size_t size);
void* safe_calloc(size_t amount, size_t size);
void* safe_realloc(void* pointer, size_t size);
void safe_free(void* pointer);
void safe_free_all();
