/***************************************************\
 pl32lib-ng, v1.05
 (c) 2022-2023 pocketlinux32, Under MPL v2.0
 pl32-memory.h: Safe memory management module header
\***************************************************/
#pragma once
#if defined(__cplusplus) && !defined(PL32CPP)
	#error Direct C header include in C++ detected. Please use pl32.hpp instead of directly using the C headers
#endif

#ifndef __cplusplus
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#endif

#define PL32LIBNG_VERSION "1.05"
#define PL32LIBNG_API_VER 1
#define PL32LIBNG_FEATURELVL 5
#define PL32LIBNG_PATCHLVL 0

// TODO: remove enum values next ABI breakage
typedef enum plmtaction {
	PLMT_GET_USEDMEM = 6,
	PLMT_GET_MAXMEM = 7,
	PLMT_SET_MAXMEM = 8,
} plmtaction_t;

typedef uint8_t byte_t;
typedef char* string_t;
typedef void* memptr_t;
typedef struct plmt plmt_t;

typedef struct plfatptr {
	memptr_t array;
	size_t size;
	bool isMemAlloc;
	plmt_t* mt;
} plfatptr_t;

typedef struct plchar {
	byte_t bytes[4];
} plchar_t;

typedef plfatptr_t plarray_t;
typedef struct plstring {
	plarray_t data;
	bool isplChar;
} plstring_t;

void plPanic(string_t msg, bool usePerror, bool developerBug);
plmt_t* plMTInit(size_t maxMemoryAlloc);
void plMTStop(plmt_t* mt);
size_t plMTMemAmnt(plmt_t* mt, plmtaction_t action, size_t size);

memptr_t plMTAlloc(plmt_t* mt, size_t size);
memptr_t plMTAllocE(plmt_t* mt, size_t size);
memptr_t plMTCalloc(plmt_t* mt, size_t amount, size_t size);
memptr_t plMTRealloc(plmt_t* mt, memptr_t pointer, size_t size);
void plMTFree(plmt_t* mt, memptr_t pointer);

void plMTFreeArray(plarray_t* array, bool is2DArray);
