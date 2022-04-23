/*********************************************\
* pl32lib, v1.00                              *
* (c)2022 pocketlinux32, Under Lesser GPLv2.1 *
* File Management Module                      *
\*********************************************/
#pragma once
#include <pl32-memory.h>

#ifdef __cplusplus
#include <cstdio>
extern "C" {
#else
#include <stdio.h>
#endif

typedef struct plfile plfile_t;

plfile_t* plFOpen(char* filename, char* mode, plgc_t* gc);
plfile_t* plFToP(FILE* pointer, char* mode, plgc_t* gc);
int plFClose(plfile_t* ptr);

size_t plFRead(void* ptr, size_t size, size_t nmemb, plfile_t* stream);
size_t plFWrite(void* ptr, size_t size, size_t nmemb, plfile_t* stream);

char plFPutC(char ch, plfile_t* stream);
char plFGetC(plfile_t* stream);

int plFPuts(char* string, plfile_t* stream);
int plFGets(char* string, int num, plfile_t* stream);

int plFSeek(plfile_t* stream, long int offset, int whence);

int plFPToFile(char* filename, plfile_t* stream);
int plFCat(plfile_t* dest, plfile_t* src, int destWhence, int srcWhence, bool closeSrc);

#ifdef __cplusplus
}
#endif
