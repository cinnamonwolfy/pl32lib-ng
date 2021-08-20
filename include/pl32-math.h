#pragma once
#include <pl32-memory.h>
#include <math.h>

typedef struct calcStruct {
	double* numbers;
	char* ops;
	size_t size[2];
} calcstruct_t;

double calculate(calcstruct_t workStruct);
double calculateString(char* workString);
