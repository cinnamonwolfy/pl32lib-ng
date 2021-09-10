#pragma once
#include <pl32-memory.h>
#ifndef __cplusplus
#include <string.h>
#else
#include <cstring>
#endif

typedef struct parsedstr {
	char** array;
	size_t size;
} parsedstr_t;

typedef struct calcstruct{
	double* numbers;
	char* ops;
	size_t size[2];
} calcstruct_t;

parsedstr_t parseString(char* input, char* delimiter);
double* calculate(calcstruct_t workStruct);
double* calculateString(char* workString);
