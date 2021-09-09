// pl32-string.c: String manipulation module
#include <pl32-string.h>

// Parses a string into an array
parsedstr_t parse(char* input, char* delimiter){
	if(delimiter == NULL){
		delimiter = malloc(1);
		*delimiter = ' ';
	}

	char** workArr = safe_malloc(2 * sizeof(char*));
	int size = 1;
	char* workPtr = strtok(input, delimiter);
	workArr[0] = workPtr;

	while((workPtr = strtok(NULL, delimiter)) != NULL){
		size++;
		void* tempArr = safe_realloc(workArr, size * sizeof(char*));
		if(!tempArr){
			parsedstr_t errStruct;
			errStruct.array = NULL;
			errStruct.size = 0;
			return errStruct;
		}
		workArr = tempArr;
		workArr[size - 1] = workPtr;
	}

	parsedstr_t returnStruct;
	returnStruct.array = workArr;
	returnStruct.size = size;

	free(delimiter);

	return returnStruct;
}
