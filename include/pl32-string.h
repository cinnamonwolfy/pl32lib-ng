#pragma once
#include <pl32-memory.h>
#ifndef __cplusplus
#include <string.h>
#else
#include <cstring>
#endif

typedef struct parsedstr{
	char** array;
	size_t size;
} parsedstr_t;

parsedstr_t parse(char* input, char* delimiter);
