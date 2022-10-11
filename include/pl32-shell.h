/*********************************************\
* pl32lib, v4.00                              *
* (c)2022 pocketlinux32, Under Lesser GPLv2.1 *
* String manipulation/Shell/Parser module     *
\*********************************************/
#pragma once
#include <pl32-memory.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PLSHVAR_NULL 0
#define PLSHVAR_INT 1
#define PLSHVAR_STRING 2
#define PLSHVAR_BOOL 3
#define PLSHVAR_FLOAT 4

typedef struct plarray {
	void* array;
	size_t size;
	bool isMemAlloc;
} plarray_t;

typedef struct plvariable {
	void* varptr;
	int type;
	char* name;
	bool isMemAlloc;
} plvariable_t;

typedef struct plfunctionptr {
	int (*function)(plarray_t*, plgc_t*);
	char* name;
} plfunctionptr_t;

void setProductStrings(char* productStr, char* srcUrl);

char* plTokenize(char* string, char** leftoverStr, plgc_t* gc);
plarray_t* plParser(char* input, plgc_t* gc);
void plShellFreeArray(plarray_t* array, bool is2DArray, plgc_t* gc);
void plShellFreeVarBuf(plarray_t* array, plgc_t* gc);

uint8_t plShellVarMgmt(plarray_t* cmdline, bool* cmdlineIsNotCommand, plarray_t* variableBuf, plgc_t* gc);
uint8_t plShellComInt(plarray_t* command, plarray_t* commandBuf, plgc_t* gc);
uint8_t plShell(char* cmdline, plarray_t* variableBuf, plarray_t* commandBuf, plgc_t** gc);

void plShellInteractive(char* prompt, bool showHelpAtStart, plarray_t* variableBuf, plarray_t* comamndBuf, plgc_t* shellGC);

#ifdef __cplusplus
}
#endif
