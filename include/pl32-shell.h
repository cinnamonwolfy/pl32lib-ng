/*****************************************************************\
 pl32lib, v4.00
 (c) 2022 pocketlinux32, Under Lesser GPLv2.1
 pl32-shell.h: String manipulation, shell and parser module header
\*****************************************************************/
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

typedef struct plvariable {
	void* varptr;
	int type;
	char* name;
	bool isMemAlloc;
} plvariable_t;

typedef struct plfunctionptr {
	int (*function)(plarray_t*, plmt_t*);
	char* name;
} plfunctionptr_t;

typedef struct plshell {
	plarray_t* commandBuffer;
	plarray_t* variableBuffer;
	plarray_t* blockBuffers;
	bool isInBlock;
	plmt_t* mt;
}

void setProductStrings(char* productStr, char* srcUrl);

char* plTokenize(char* string, char** leftoverStr, plmt_t* mt);
plarray_t* plParser(char* input, plmt_t* mt);
uint8_t plShell(char* cmdline, plarray_t* variableBuf, plarray_t* commandBuf, plmt_t** mt);

void plShellInteractive(char* prompt, bool showHelpAtStart, plarray_t* variableBuf, plarray_t* comamndBuf, plmt_t* shellGC);

#ifdef __cplusplus
}
#endif
