/********************************************\
* pl32lib, v0.02                             *
* (c)2021 pocketlinux32, Under Lesser GPLv3  *
* String manipulation/parsing module         *
* Warning: unfinished!                       *
\********************************************/
#include <pl32-shell.h>

plfunctionptr_t* commands = NULL;
size_t commandAmnt = 0;

// Wrapper for ISO C function strtok() that copies the output of strtok() into a memory-allocated buffer
char* plGCAllocStrtok(char* input, char* delimiter, plgc_t* gc){
	char* returnPtr = NULL;
	char* tempPtr;

	if((tempPtr = strtok(input, delimiter)) != NULL){
		returnPtr = plGCAlloc(gc, (strlen(tempPtr) + 1) * sizeof(char));
		strcpy(returnPtr, tempPtr);
	}

	return returnPtr;
};

// Parses a string into an array
plarray_t* plParser(char* input, plgc_t* gc){
	plarray_t* returnStruct = plGCAlloc(gc, sizeof(plarray_t));
	returnStruct->size = 1;
	returnStruct->array = plGCAlloc(gc, 2 * sizeof(char*));

	char* tempPtr = plGCAllocStrtok(input, " \n", gc);
	((char**)returnStruct->array)[0] = tempPtr;

	while((tempPtr = plGCAllocStrtok(NULL, " \n", gc)) != NULL){
		returnStruct->size++;
		char** tempArrPtr = plGCRealloc(gc, returnStruct->array, returnStruct->size * sizeof(char*));

		if(!tempArrPtr){
			for(int i = 0; i < returnStruct->size; i++){
				plGCFree(gc, ((char**)returnStruct->array)[i]);
			}

			plGCFree(gc, returnStruct->array);
			plGCFree(gc, returnStruct);

			return NULL;
		}

		returnStruct->array = tempArrPtr;
		((char**)returnStruct->array)[returnStruct->size - 1] = tempPtr;
	}

	return returnStruct;
}

void plPrintTokenizedStr(plarray_t* tokstr){
	printf("Listing tokenized string list:\n");
	for(int i = 0; i < tokstr->size; i++){
		printf("	token[%d]: %s\n", i, ((char**)tokstr->array)[i]);
	}
}

int plShellAddFunction(plfunctionptr_t* functionPtr, plgc_t* gc){
	void* tempPtr;

	if(commands == NULL){
		tempPtr = plGCAlloc(gc, sizeof(plfunctionptr_t) * 2);
	}else if(commandAmnt >= 2){
		tempPtr = plGCRealloc(gc, commands, sizeof(plfunctionptr_t) * commandAmnt + 1);
	}

	if(!tempPtr)
		return 1;

	if(commands == NULL || commands >= 2)
		commands = tempPtr;

	commands[commandAmnt].function = functionPtr->function;
	commands[commandAmnt].name = functionPtr->name;
	commandAmnt++;

	return 0;
}
