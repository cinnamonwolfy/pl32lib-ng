/********************************************\
* pl32lib, v0.04                             *
* (c)2022 pocketlinux32, Under Lesser GPLv3  *
* String manipulation/Shell/Parser module    *
* Warning: unfinished!                       *
\********************************************/
#pragma once
#include <pl32-memory.h>
#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct plarray {
	void* array;
	size_t size;
} plarray_t;

typedef struct plfunctionptr {
	int (*function)(plarray_t*);
	char* name;
} plfunctionptr_t;

char* plGCAllocStrtok(char* input, char* delimiter, plgc_t* gc);
plarray_t* plParser(char* input, plgc_t* gc);

int plShellAddFunction(plfunctionptr_t* functionPtr, plgc_t* gc);
void plShellRemoveFunction(char* name, plgc_t* gc);

uint8_t plShell(char* command, plgc_t* gc);

void plShellInteractive(char* prompt);

#ifdef __cplusplus
}
#endif
