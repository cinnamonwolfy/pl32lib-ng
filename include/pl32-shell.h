/********************************************\
* pl32lib, v0.01                             *
* (c)2021 pocketlinux32, Under Lesser GPLv3  *
* String manipulation/Shell/Parser module    *
* Warning: unfinished!                       *
\********************************************/
#pragma once
#include <pl32-memory.h>

typedef struct pltokenizedstr pltokenizedstr_t;
typedef struct plfunctionptr plfunctionptr_t;

char* plSafeMallocStrtok(char* input, char* delimiter);
pltokenizedstr_t plParser(char* input);

plfunctionptr_t plCreateFunctionPointer(int (*function)(int, char*), char* name); // Unfinished

int plShellAddFunction(plfunctionptr_t functionPtr); // Undefined
void plShellRemoveFunction(char* name); // Undefined

int plShell(char* command); // Undefined
int plShellFile(char* filename); // Undefined

void plShellInteractive(char* prompt); // Undefined
