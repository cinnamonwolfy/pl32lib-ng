/****************************************************\
 pl32lib-ng, v1.04
 (c) 2022-2023 pocketlinux32, Under MPL v2.0
 pl32-memory.h: Safe memory management module headers
\****************************************************/
#pragma once
#ifdef __cplusplus
extern "C" {
#else
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#endif

#define PL32LIBNG_VERSION "1.04"
#define PL32LIBNG_API_VER 1
#define PL32LIBNG_FEATURELVL 4
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
	uint8_t bytes[4];
} plchar_t;

typedef plfatptr_t plarray_t;
typedef struct plstring {
	plarray_t data;
	bool isplChar;
} plstring_t;

void plPanic(string_t msg, bool usePerror, bool devbug);
plmt_t* plMTInit(size_t maxMemoryAlloc);
void plMTStop(plmt_t* mt);
size_t plMTMemAmnt(plmt_t* mt, plmtaction_t action, size_t size);

memptr_t plMTAlloc(plmt_t* mt, size_t size);
memptr_t plMTAllocE(plmt_t* mt, size_t size);
memptr_t plMTCalloc(plmt_t* mt, size_t amount, size_t size);
memptr_t plMTRealloc(plmt_t* mt, memptr_t pointer, size_t size);
void plMTFree(plmt_t* mt, memptr_t pointer);

void plMTFreeArray(plarray_t* array, bool is2DArray);

#ifdef __cplusplus
}
#endif
