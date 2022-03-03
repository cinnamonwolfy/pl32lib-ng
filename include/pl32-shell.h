/********************************************\
* pl32lib, v0.01                             *
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

typedef struct pltokenizedstr pltokenizedstr_t;
typedef struct plfunctionptr plfunctionptr_t;

char* plGCAllocStrtok(char* input, char* delimiter, plgc_t* gc);
pltokenizedstr_t plParser(char* input, plgc_t* gc);

plfunctionptr_t plCreateFunctionPointer(int (*function)(int, char**), char* name); // Unfinished

int plShellAddFunction(plfunctionptr_t functionPtr); // Undefined
void plShellRemoveFunction(char* name); // Undefined

int plShell(char* command); // Undefined
int plShellFile(char* filename); // Undefined

void plShellInteractive(char* prompt); // Undefined

#ifdef __cplusplus
}
#endif
