#pragma once
#include <pl32-memory.h>
#include <pl32-string.h>

#ifndef __cplusplus
#include <math.h>
#else
#include <cmath>
#endif

typedef struct calcstruct{
	double* numbers;
	char* ops;
	size_t size[2];
} calcstruct_t;

double calculate(calcstruct_t workStruct);
double calculateString(char* workString);
