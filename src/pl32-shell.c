/******************************************************\
* pl32lib, v4.00                                       *
* (c)2022 pocketlinux32, Under Lesser GPLv2.1 or later *
* String manipulation/Shell/Parser module              *
\******************************************************/
#include <pl32-shell.h>

char productString[4096] = "PocketLinux Shell, (c)2022 pocketlinux32";
char srcUrlString[4096] = "";

void setProductStrings(char* productStr, char* srcUrl){
	strcpy(productString, productStr);
	if(srcUrl)
		strcpy(srcUrlString, srcUrl);
}

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

	// If string starts with a space and string pointer is lower than the search limit,
	// keep increasing string pointer until it equals tempPtr[1]
	if(*string == ' '){
		while(*string == ' ' && string < searchLimit)
			string++;

		if(tempPtr[1] < string)
			tempPtr[1] = strchr(string, ' ');
	}

	// If there are no string in quotes, or if the quotes come after a whitespace,
	// assign the beginning of the string to be returned as the pointer of the given
	// string and the end of the string as the next whitespase
	if((!tempPtr[0] && tempPtr[1]) || (tempPtr[1] && tempPtr[1] < tempPtr[0])){
		startPtr = string;
		endPtr = tempPtr[1];
	// Else, if the beginning of the given string begins with a quote, assign the beginning
	// of the string to be returned as the opening quote + 1 and the end of the string as the
	// closing quote
	}else if(tempPtr[0] && tempPtr[0] == string){
		startPtr = tempPtr[0] + 1;
		endPtr = strchr(tempPtr[0] + 1, '"');
	}

	size_t strSize = (endPtr - startPtr);

	// If there's no whitespace or quotes in the given string, return the given string.
	if(!startPtr || !endPtr || !strSize){
		if(strlen(string) != 0){
			strSize = strlen(string);
			*leftoverStr = NULL;
			startPtr = string;
		}else{
			return NULL;
		}
	}else{
	// Else what remains of the given string into the leftover variable
		*leftoverStr = endPtr+1;
	}

	// Allocate memory for return string, copy the selected part of the string,
	// and null-terminate the return string
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

	// First token
	char* tempPtr = plTokenize(input, &leftoverStr, gc);
	((char**)returnStruct->array)[0] = tempPtr;

	// Keep tokenizing until there is no more string left to tokenize
	while((tempPtr = plTokenize(leftoverStr, &leftoverStr, gc)) != NULL){
		returnStruct->size++;
		char** tempArrPtr = plGCRealloc(gc, returnStruct->array, returnStruct->size * sizeof(char*));

		if(!tempArrPtr){
			for(int i = 0; i < returnStruct->size; i++)
				plGCFree(gc, ((char**)returnStruct->array)[i]);

			plGCFree(gc, returnStruct->array);
			plGCFree(gc, returnStruct);

			return NULL;
		}

		returnStruct->array = tempArrPtr;
		((char**)returnStruct->array)[returnStruct->size - 1] = tempPtr;
	}

	return returnStruct;
}

// Frees a plarray_t
void plShellFreeArray(plarray_t* array, bool is2DArray, plgc_t* gc){
	if(is2DArray){
		for(int i = 0; i < array->size; i++)
			plGCFree(gc, ((void**)array->array)[i]);
	}
	plGCFree(gc, array->array);
	plGCFree(gc, array);
}

// Variable Manager
uint8_t plShellVarMgmt(plarray_t* cmdline, bool* cmdlineIsNotCommand, plarray_t* variableBuf, plgc_t* gc){
	if(!gc || !cmdline || !cmdlineIsNotCommand || !variableBuf)
		return 255;

	char** array = cmdline->array;
	int assignVal = -1;
	plvariable_t* workVarBuf = variableBuf->array;

	if(strchr(array[0], '=') != NULL || strchr(array[1], '=') == array[1]){
		*cmdlineIsNotCommand = true;
		if(strchr(array[0], '=') != NULL)
			assignVal = 0;
		else if(strchr(array[1], '=') == array[1])
			assignVal = 2;
		else
			assignVal = 1;
	}


	int i = 0, j = -1;
	while(i < cmdline->size && strchr(array[i], '$') == NULL)
		i++;

        if(i < cmdline->size && (strchr(array[i], '$') == array[i] || strchr(array[i], '$') == array[i] + 1)){
		char* workVar = strchr(array[i], '$') + 1;
		j = 0;

		while(strcmp(workVar, workVarBuf[j].name) != 0 && j < variableBuf->size)
			j++;

		if(j == variableBuf->size){
			printf("%s: Non-existent variable ", workVar);
			return 255;
		}
	}

	if(!*cmdlineIsNotCommand && j > -1){
		void* valToExpand = workVarBuf[j].varptr;
		char holderString[1024] = "";
		char* outputString = NULL;

		switch(workVarBuf[j].type){
			case PLSHVAR_INT: ;
				snprintf(holderString, 1023, "%d", *((int*)valToExpand));
				break;
			case PLSHVAR_STRING: ;
				outputString = (char*)valToExpand;
				break;
			case PLSHVAR_BOOL: ;
				bool tempBool = *((bool*)valToExpand);

				if(tempBool)
					strcmp(holderString, "true");
				else
					strcmp(holderString, "false");
				break;
			case PLSHVAR_FLOAT: ;
				snprintf(holderString, 1023, "%f", *((float*)valToExpand));
				break;
		}

		if(!outputString)
			outputString = holderString;

		size_t sizeOfOut = strlen(outputString);
		plGCFree(gc, array[i]);
		array[i] = plGCAlloc(gc, sizeOfOut);
		memcpy(array[i], outputString, sizeOfOut);
		array[i][sizeOfOut] = 0;
	}else if(assignVal > -1){
		char* varToAssign = NULL;
		char* valToAssign = NULL;
		char* workVar = strchr(array[0], '=');
		int type = PLSHVAR_NULL;

		if(assignVal < 2 && workVar){
			size_t sizeToCopy = workVar - array[0];
			varToAssign = plGCAlloc(gc, sizeToCopy + 1);
			memcpy(varToAssign, array[0], sizeToCopy);
			varToAssign[sizeToCopy] = 0;
		}else{
			varToAssign = array[0];
		}

		if(j == -1){
			switch(assignVal){
				case 0: ;
					size_t sizeToCopy = (array[0] + strlen(array[0])) - workVar;
					valToAssign = plGCAlloc(gc, sizeToCopy + 1);
					memcpy(valToAssign, workVar + 1, sizeToCopy);
					valToAssign[sizeToCopy] = 0;
					break;
				case 1: ;
					if(workVar){
						valToAssign = array[1];
					}else{
						size_t sizeToCopy = strlen(array[1]) - 1;
						valToAssign = plGCAlloc(gc, sizeToCopy + 1);
						memcpy(valToAssign, workVar + 1, sizeToCopy);
						valToAssign[sizeToCopy] = 0;
					}
					break;
				case 2: ;
					varToAssign = array[2];
					break;
			}
		}else{
			valToAssign = workVarBuf[j].varptr;
		}

		i = 0;
		while(workVarBuf[i].varptr == NULL && i < variableBuf->size)
			i++;

		if(i == variableBuf->size && !variableBuf->isMemAlloc){
			printf("Error: Variable buffer is full\n");
			return ENOMEM;
		}else{
			if(i == variableBuf->size){
				void* tempPtr = plGCRealloc(gc, variableBuf->array, (variableBuf->size + 1) * sizeof(plvariable_t));
				if(!tempPtr)
					return ENOMEM;

				variableBuf->array = tempPtr;
				workVarBuf = variableBuf->array;
			}

			if(j == -1){
				char* leftoverStr = NULL;
				int number;
				bool boolean;
				float decimal;
				void* pointer;

				number = strtol(valToAssign, &leftoverStr, 10);
				if(leftoverStr != NULL && *leftoverStr != '\0'){
					decimal = strtof(valToAssign, &leftoverStr);
					if(leftoverStr != NULL && *leftoverStr != '\0'){
						if(strcmp("true", valToAssign) == 0){
							boolean = true;
							if(!(pointer = plGCAlloc(gc, sizeof(bool)))){
								printf("Error: Out of memory\n");
								return ENOMEM;
							}

							*((bool*)pointer) = boolean;
							type = PLSHVAR_BOOL;
						}else if(strcmp("false", valToAssign) == 0){
							boolean = false;
							if(!(pointer = plGCAlloc(gc, sizeof(bool)))){
								printf("Error: Out of memory\n");
								return ENOMEM;
							}


							*((bool*)pointer) = boolean;
							type = PLSHVAR_BOOL;
						}else{
							if(!(pointer = plGCAlloc(gc, (strlen(valToAssign) + 1) * sizeof(char)))){
								printf("Error: Out of memory\n");
								return ENOMEM;
							}

							strcpy(pointer, valToAssign);
							type = PLSHVAR_STRING;
						}
					}else{
						if(!(pointer = plGCAlloc(gc, sizeof(float)))){
							printf("Error: Out of memory\n");
							return ENOMEM;
						}

						*((float*)pointer) = decimal;
						type = PLSHVAR_FLOAT;
					}
				}else{
					if(!(pointer = plGCAlloc(gc, sizeof(int)))){
						printf("Error: Out of memory\n");
						return ENOMEM;
					}

					*((int*)pointer) = number;
					type = PLSHVAR_INT;
				}
			}else{
				workVarBuf[i].varptr = valToAssign;
			}
		}
	}
}

// Command Interpreter
uint8_t plShellComInt(plarray_t* command, plarray_t* commandBuf, plgc_t* gc){
	if(!gc || !command)
		return 1;

	char** array = command->array;
	int retVar = 0;

	if(strcmp(array[0], "print") == 0){
		if(command->size < 2)
			return 1;

		for(int i = 1; i < command->size - 1; i++)
			printf("%s ", array[i]);

		printf("%s\n", array[command->size - 1]);
	}else if(strcmp(array[0], "clear") == 0){
		printf("\033c");
	}else if(strcmp(array[0], "exit") == 0){
		int tempNum = 0;
		char* pointer;
		if(command->size < 2)
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
			retVar = ((plfunctionptr_t*)commandBuf->array)[i].function(command, gc);
		}
	}

	return retVar;
}

// Complete Shell Interpreter
uint8_t plShell(char* cmdline, plarray_t* variableBuf, plarray_t* commandBuf, plgc_t** gc){
	if(!gc || !*gc)
		return 1;

	bool cmdlineIsNotCommand = false;
	plarray_t* parsedCmdLine = plParser(cmdline, *gc);

	if(!parsedCmdLine)
		return 1;

	if(strchr(cmdline, '$') || strchr(cmdline, '='))
		plShellVarMgmt(parsedCmdLine, &cmdlineIsNotCommand, variableBuf, *gc);

	char** array = parsedCmdLine->array;
	int retVar = 0;

	if(!cmdlineIsNotCommand){
		if(strcmp(array[0], "version") == 0 || strcmp(array[0], "help") == 0){
			printf("%s\n", productString);
			if(strlen(srcUrlString) > 0)
				printf("src at %s\n", srcUrlString);

			printf("\npl32lib v%s, (c)2022 pocketlinux32, Under LGPLv2.1\n", PL32LIB_VERSION);
			printf("src at https://github.com/pocketlinux32/pl32lib\n");
			if(strcmp(array[0], "help") == 0){
				printf("Built-in commands: print, clear, exit, show-memusg, reset-mem, version, help\n");
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
		}else if(strcmp(array[0], "show-memusg") == 0){
			printf("%ld bytes free\n", plGCMemAmnt(*gc, PLGC_GET_MAXMEM, 0) -  plGCMemAmnt(*gc, PLGC_GET_USEDMEM, 0));
		}else if(strcmp(array[0], "reset-mem") == 0){
			size_t size = plGCMemAmnt(*gc, PLGC_GET_MAXMEM, 0);
			plGCStop(*gc);
			*gc = plGCInit(size);
			printf("Memory has been reset\n");
			return retVar;
		}else{
			retVar = plShellComInt(parsedCmdLine, commandBuf, *gc);
		}
	}

	plShellFreeArray(parsedCmdLine, true, *gc);
	return retVar;
}

// Interactive frontend to plShellFrontEnd()
void plShellInteractive(char* prompt, bool showHelpAtStart, plarray_t* variableBuf, plarray_t* commandBuf, plgc_t* shellGC){
	bool loop = true;
	bool showRetVal = false;

	if(!shellGC)
		shellGC = plGCInit(8 * 1024 * 1024);

	if(!prompt)
		prompt = "(cmd) # ";

	if(showHelpAtStart){
		plShell("help", variableBuf, commandBuf, &shellGC);
		plShell("show-memusg", variableBuf, commandBuf, &shellGC);
	}

	while(loop){
		char cmdline[4096] = "";
		int retVal = 0;
		printf("%s", prompt);
		scanf("%4096[^\n]", cmdline);
		getchar();

		if(strcmp(cmdline, "exit-shell") == 0 || feof(stdin)){
			loop = false;
		}else if(strcmp(cmdline, "show-exitval") == 0){
			if(showRetVal){
				showRetVal = false;
			}else{
				showRetVal = true;
			}
		}else if(strlen(cmdline) > 0){
			retVal = plShell(cmdline, variableBuf, commandBuf, &shellGC);
		}

		if(showRetVal)
			printf("\nretVal = %d\n", retVal);
	}

	if(feof(stdin))
		printf("\n");

	plGCStop(shellGC);
}
