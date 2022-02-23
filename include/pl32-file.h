/********************************************\
* pl32lib, v0.01                             *
* (c)2021 pocketlinux32, Under Lesser GPLv3  *
* File Management Module                     *
* Warning: unfinished!                       *
\********************************************/
#pragma once
#include <pl32-memory.h>

typedef struct plfile plfile_t;

plfile_t* plFOpen(char* filename, char* mode);
int plFClose(plfile_t* ptr);

size_t plFRead(const void* ptr, size_t size, size_t nmemb, plfile_t* stream);
size_t plFWrite(const void* ptr, size_t size, size_t nmemb, plfile_t* stream);

