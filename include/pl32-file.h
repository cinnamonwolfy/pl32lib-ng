/********************************************\
 pl32lib-ng, v1.00
 (c) 2022 pocketlinux32, Under MPL v2.0
 pl32-file.h: File management module header
\********************************************/
#pragma once
#include <pl32-memory.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct plfile plfile_t;

plfile_t* plFOpen(char* filename, char* mode, plmt_t* mt);
plfile_t* plFToP(FILE* pointer, char* mode, plmt_t* mt);
int plFClose(plfile_t* ptr);

size_t plFRead(void* ptr, size_t size, size_t nmemb, plfile_t* stream);
size_t plFWrite(void* ptr, size_t size, size_t nmemb, plfile_t* stream);

int plFPutC(char ch, plfile_t* stream);
int plFGetC(plfile_t* stream);

int plFPuts(char* string, plfile_t* stream);
char* plFGets(char* string, int num, plfile_t* stream);

int plFSeek(plfile_t* stream, long int offset, int whence);
size_t plFTell(plfile_t* stream);

int plFPToFile(char* filename, plfile_t* stream);
void plFCat(plfile_t* dest, plfile_t* src, int destWhence, int srcWhence, bool closeSrc);

#ifdef __cplusplus
}
#endif
