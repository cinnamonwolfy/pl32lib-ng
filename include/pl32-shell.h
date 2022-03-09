/********************************************\
* pl32lib, v0.02                             *
* (c)2021 pocketlinux32, Under Lesser GPLv3  *
* String manipulation/Shell/Parser module    *
* Warning: unfinished!                       *
\********************************************/
#pragma once
#include <pl32-memory.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct plarray {
	void* array;
	size_t size;
} plarray_t;

typedef struct plfunctionptr {
	int (*function)(int, char**);
	char* name;
} plfunctionptr_t;

char* plGCAllocStrtok(char* input, char* delimiter, plgc_t* gc);
plarray_t* plParser(char* input, plgc_t* gc);

int plShellAddFunction(plfunctionptr_t* functionPtr);
void plShellRemoveFunction(char* name);

int plShell(char* command); // Undefined

void plShellInteractive(char* prompt); // Undefined

#ifdef __cplusplus
}
#endif
