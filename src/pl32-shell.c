/*********************************************\
* pl32lib, v0.05                              *
* (c)2022 pocketlinux32, Under Lesser GPLv2.1 *
* String manipulation/parsing module          *
\*********************************************/
#include <pl32-shell.h>

// Tokenizes string in a similar way an interactive command line would
char* plTokenize(char* string, char** leftoverStr, plgc_t* gc){
	if(!string || !gc)
		return NULL;

	char* tempPtr[2] = { strchr(string, '"'), strchr(string, ' ') };
	char* searchLimit = string + strlen(string);
	char* startPtr = NULL;
	char* endPtr = NULL;
	char* retPtr;

	if(strlen(string) == 0){
		return NULL;
	}

	if(*string == ' '){
		while(*string == ' ' && string < searchLimit) string++;
		if(tempPtr[1] < string){
			tempPtr[1] = strchr(string, ' ');
		}
	}

	if((!tempPtr[0] && tempPtr[1]) || (tempPtr[1] && tempPtr[1] < tempPtr[0])){
		startPtr = string;
		endPtr = tempPtr[1];
	}else if(tempPtr[0] && tempPtr[0] == string){
		startPtr = tempPtr[0] + 1;
		endPtr = strchr(tempPtr[0] + 1, '"');
	}

	size_t strSize = (endPtr - startPtr);

	if(!startPtr || !endPtr || !strSize){
		if(strlen(string) != 0){
			strSize = strlen(string);
			*leftoverStr = NULL;
			startPtr = string;
		}else{
			return NULL;
		}
	}else{
		*leftoverStr = endPtr+1;
	}

	retPtr = plGCAlloc(gc, (strSize + 1) * sizeof(char));
	memcpy(retPtr, startPtr, strSize);

	retPtr[strSize] = '\0';
	return retPtr;
}

// Parses a string into an array
plarray_t* plParser(char* input, plgc_t* gc){
	if(!input || !gc)
		return NULL;

	char* leftoverStr;
	plarray_t* returnStruct = plGCAlloc(gc, sizeof(plarray_t));
	returnStruct->size = 1;
	returnStruct->array = plGCAlloc(gc, 2 * sizeof(char*));

	char* tempPtr = plTokenize(input, &leftoverStr, gc);
	((char**)returnStruct->array)[0] = tempPtr;

	while((tempPtr = plTokenize(leftoverStr, &leftoverStr, gc)) != NULL){
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

void plShellFreeArray(plarray_t* array, bool isStringArray, plgc_t* gc){
	if(isStringArray){
		for(int i = 0; i < array->size; i++)
			plGCFree(gc, ((char**)array->array)[i]);
	}
	plGCFree(gc, array->array);
	plGCFree(gc, array);
}

// Command Interpreter
uint8_t plShell(char* command, plarray_t* commandBuf, plgc_t* gc){
	plarray_t* parsedCmdLine = plParser(command, gc);

	if(!parsedCmdLine)
		return 1;

	char** array = parsedCmdLine->array;
	int retVar = 0;

	if(strcmp(array[0], "print") == 0){
		if(parsedCmdLine->size < 2)
			return 1;

		for(int i = 1; i < parsedCmdLine->size - 1; i++)
			printf("%s ", array[i]);

		printf("%s\n", array[parsedCmdLine->size - 1]);
	}else if(strcmp(array[0], "clear") == 0){
		printf("\033c");
	}else if(strcmp(array[0], "version") == 0 || strcmp(array[0], "help") == 0){
		printf("PocketLinux Shell, (c)2022 pocketlinux32\n");
		printf("pl32lib v%s, Under Lesser GPLv3\n", PL32LIB_VERSION);
		printf("src at https://github.com/pocketlinux32/pl32lib\n");
		if(strcmp(array[0], "help") == 0){
			printf("Built-in commands: print, clear, version, help, exit\n");
			if(!commandBuf || commandBuf->size == 0){
				printf("No user-defined commands loaded\n");
			}else{
				printf("%ld user-defined commands loaded\n", commandBuf->size);
				printf("User-defined commands: ");
				for(int i = 0; i < commandBuf->size - 1; i++)
					printf("%s, ", ((plfunctionptr_t*)commandBuf->array)[i].name);

				printf("%s\n", ((plfunctionptr_t*)commandBuf->array)[commandBuf->size - 1].name);
			}
		}
	}else if(strcmp(array[0], "exit") == 0){
		int tempNum = 0;
		char* pointer;
		if(parsedCmdLine->size < 2)
			exit(tempNum);

		exit(strtol(array[0], &pointer, 10));
	}else{
		int i = 0;

		if(!commandBuf || commandBuf->size == 0){
			printf("%s: command not found\n", array[0]);
			return 255;
		}

		while(strcmp(((plfunctionptr_t*)commandBuf->array)[i].name, array[0]) != 0 && i < commandBuf->size - 1){
			i++;
		}

		if(strcmp(((plfunctionptr_t*)commandBuf->array)[i].name, array[0]) != 0){
			printf("%s: command not found\n", array[0]);
			return 255;
		}else{
			retVar = ((plfunctionptr_t*)commandBuf->array)[i].function(parsedCmdLine, gc);
		}
	}

	plShellFreeArray(parsedCmdLine, true, gc);
	return retVar;
}

// Interactive frontend to plShell()
void plShellInteractive(char* prompt, plarray_t* commandBuf, plgc_t* shellGC){
	bool loop = true;

	if(!shellGC)
		shellGC = plGCInit(8 * 1024 * 1024);

	if(!prompt)
		prompt = "(cmd) # ";

	plShell("help", commandBuf, shellGC);
	while(loop){
		char cmdline[4096] = "";
		printf("%s", prompt);
		scanf("%4096[^\n]", cmdline);
		getchar();

		if(strcmp(cmdline, "exit-shell") == 0 || feof(stdin)){
			loop = false;
		}else if(strlen(cmdline) > 0){
			plShell(cmdline, commandBuf, shellGC);
		}
	}

	if(feof(stdin))
		printf("\n");

	plGCStop(shellGC);
}
