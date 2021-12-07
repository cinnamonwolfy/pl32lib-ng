#pragma once
#include <pl32-memory.h>
#ifndef __cplusplus
#include <string.h>
#else
#include <cstring>
#endif

typedef struct pltokenizedstr pltokenizedstr_t;
typedef struct plfunctionptr plfunctionptr_t;

char* plSafeMallocStrtok(char* input, char* delimiter);
pltokenizedstr_t plParser(char* input);

plfunctionptr_t plCreateFunctionPointer(int (*function)(int, char*), char* name);

int plShellAddFunction(plfunctionptr_t functionPtr);
void plShellRemoveFunction(char* name);

int plShell(char* command);
int plShellFile(char* filename);

void plShellInteractive(char* prompt);
