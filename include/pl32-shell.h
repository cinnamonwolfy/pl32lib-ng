/*********************************************\
* pl32lib, v2.00                              *
* (c)2022 pocketlinux32, Under Lesser GPLv2.1 *
* String manipulation/Shell/Parser module     *
\*********************************************/
#pragma once
#include <pl32-memory.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct plarray {
	void* array;
	size_t size;
} plarray_t;

typedef struct plfunctionptr {
	int (*function)(plarray_t*, plgc_t*);
	char* name;
} plfunctionptr_t;

char* plTokenize(char* string, char** leftoverStr, plgc_t* gc);
plarray_t* plParser(char* input, plgc_t* gc);
void plShellFreeArray(plarray_t* array, bool is2DArray, plgc_t* gc);

uint8_t plShell(plarray_t* command, plarray_t* commandBuf, plgc_t* gc);
uint8_t plShellFrontEnd(char* cmdline, plarray_t* variableBuf, plarray_t* commandBuf, plgc_t** gc);

void plShellInteractive(char* prompt, bool showHelpAtStart, plarray_t* variableBuf, plarray_t* comamndBuf, plgc_t* shellGC);

#ifdef __cplusplus
}
#endif
