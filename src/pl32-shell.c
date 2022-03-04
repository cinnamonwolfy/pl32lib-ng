/********************************************\
* pl32lib, v0.01                             *
* (c)2021 pocketlinux32, Under Lesser GPLv3  *
* String manipulation/Shell/Parser module    *
* Warning: unfinished!                       *
\********************************************/
#include <pl32-shell.h>

// Function Pointer
struct plfunctionptr {
	int (*function)(int, char**);
	char* name;
};

// Wrapper for ISO C function strtok() that copies the output of strtok() into a memory-allocated buffer
char* plGCAllocStrtok(char* input, char* delimiter, plgc_t* gc){
	char* returnPtr = NULL;
	char* tempPtr;

	if((tempPtr = strtok(input, delimiter)) != NULL){
		returnPtr = plGCAlloc(gc, (strlen(tempPtr) + 1) * sizeof(char));
		strcpy(tempPtr, returnPtr);
	}

	return returnPtr;
};

// Parses a string into an array
pltokenizedstr_t plParser(char* input, plgc_t* gc){
	pltokenizedstr_t returnStruct;
	returnStruct.size = 1;
	returnStruct.array = plGCAlloc(gc, 2 * sizeof(char*));

	char* tempPtr = plGCAllocStrtok(input, " \n", gc);
	returnStruct.array[0] = tempPtr;

	while((tempPtr = plGCAllocStrtok(NULL, " \n", gc)) != NULL){
		returnStruct.size++;
		char** tempArrPtr = plGCRealloc(gc, returnStruct.array, returnStruct.size * sizeof(char*));

		if(!tempArrPtr){
			for(int i = 0; i < returnStruct.size; i++){
				plGCFree(gc, returnStruct.array[i]);
			}

			plGCFree(gc, returnStruct.array);
			returnStruct.array = NULL;
			returnStruct.size = ENOMEM;

			return returnStruct;
		}

		returnStruct.array = tempArrPtr;
		returnStruct.array[returnStruct.size - 1] = tempPtr;
	}

	return returnStruct;
}

void plPrintTokenizedStr(pltokenizedstr_t tokstr){
	printf("Listing tokenized string list:\n");
	for(int i = 0; i < tokstr.size; i++){
		printf("	token[%d]: %s\n", i, tokstr.array[i]);
	}
}

// Creates an object containing function pointer
plfunctionptr_t plCreateFunctionPointer(int (*function)(int, char**), char* name){
	plfunctionptr_t returnStruct;
	returnStruct.function = function;
	returnStruct.name = name;

	return returnStruct;
}

int plShellAddFunction(plfunctionptr_t functionPtr){
	
}
