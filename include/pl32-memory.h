/****************************************************\
 pl32lib-ng, v1.01
 (c) 2022 pocketlinux32, Under MPL v2.0
 pl32-memory.h: Safe memory management module headers
\****************************************************/
#pragma once
#ifdef __cplusplus
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cerrno>
#include <cstring>

extern "C" {
#else
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#endif

#define PL32LIBNG_VERSION "1.02"
#define PL32LIBNG_API_VER 1
#define PL32LIBNG_FEATURELVL 2
#define PL32LIBNG_PATCHLVL 0

#define PLMT_STOP 1
#define PLMT_SEARCHPTR 2
#define PLMT_ADDPTR 3
#define PLMT_RMPTR 4
#define PLMT_REALLOC 5
#define PLMT_GET_USEDMEM 6
#define PLMT_GET_MAXMEM 7
#define PLMT_SET_MAXMEM 8

typedef char byte_t;
typedef char* string_t;
typedef void* memptr_t;

typedef struct plmt plmt_t;

typedef struct plarray {
	memptr_t array;
	size_t size;
	bool isMemAlloc;
	plmt_t* mt;
} plarray_t;

plmt_t* plMTInit(size_t maxMemoryAlloc);
void plMTStop(plmt_t* mt);
size_t plMTMemAmnt(plmt_t* mt, int action, size_t size);

memptr_t plMTAlloc(plmt_t* mt, size_t size);
memptr_t plMTAllocE(plmt_t* mt, size_t size);
memptr_t plMTCalloc(plmt_t* mt, size_t amount, size_t size);
memptr_t plMTRealloc(plmt_t* mt, memptr_t pointer, size_t size);
void plMTFree(plmt_t* mt, memptr_t pointer);

void plMTFreeArray(plarray_t* array, bool is2DArray);

#ifdef __cplusplus
}
#endif
