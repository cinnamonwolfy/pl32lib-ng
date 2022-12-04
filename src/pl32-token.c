/*****************************************************************\
 pl32lib-ng, v0.95
 (c) 2022 pocketlinux32, Under Lesser GPLv2.1
 pl32-token.c: String manipulation and parser module header
\*****************************************************************/
#include <pl32-token.h>

/* A thread-safe reimplementation of Standard C function strtok */
char* plStrtok(char* str, char* delim, char** leftoverStr, plmt_t* mt){
	if(str == NULL || delim == NULL || leftoverStr == NULL || mt == NULL)
		return NULL;

	char* retPtr;
	char* endPtr = strstr(str, delim);
	char* searchLimit = str + strlen(str);
	size_t delimSize = strlen(delim);

	if(endPtr == NULL)
		endPtr = searchLimit;

	/* If endPtr also equals str, then keep increasing pointer address by the size of *\
	\* delim. If the entire string consists of delim, then return a null pointer      */
	if(endPtr == str){
		while((endPtr = strstr(str, delim)) == str)
			str += delimSize;

		if(endPtr == NULL){
			*leftoverStr = NULL;
			return NULL;
		}
	}

	/* Copies the memory block into the return pointer */
	size_t strSize = endPtr - str;
	retPtr = plMTAlloc(mt, strSize + 1);
	memcpy(retPtr, string, strSize);
	retPtr[strSize] = '\0';

	if(endPtr != searchLimit)
		*leftoverStr = endPtr + delimSize;
	else
		*leftoverStr = NULL;

	return retPtr;
}

char* plTokenizeStrtok(char* string, char** leftoverStr, plmt_t* mt){
	if(string == NULL || leftoverStr == NULL || mt == NULL)
		return NULL;

	if(strlen(string) == 0)
		return NULL;

	char* tempPtr[2] = { strchr(string, '"'), strchr(string, '\'') };
	char* spaceChar = strchr(string, ' ');
	bool noQuotesFound = (tempPtr[0] == NULL && tempPtr[1] == NULL);
	bool noEndQuotesFound = (tempPtr[0] != NULL && strchr(tempPtr[0] + 1, '"') == NULL) || (tempPtr[1] != NULL && strchr(temPtr[1] + 1, '\'');
	bool spaceComesFirst = ((tempPtr[0] != NULL && spaceChar < tempPtr[0]) || (tempPtr[1] != NULL && spaceChar < tempPtr[1]));
	bool literalBeforeBasicStr = (tempPtr[1] != NULL && tempPtr[1] < tempPtr[0]);

	if(noQuotesFound || noEndQuotesFound || (spaceChar != NULL && spaceComesFirst)){
		return plStrtok(string, " ", leftoverStr, mt);
	}else{
		char* retPtr = NULL;
		char* searchLimit = string + strlen(string);

		if(literalBeforeBasicStr && strchr(tempPtr[1] + 1, '\'') != NULL){
			retPtr = plStrtok(tempPtr[1] + 1, "'", leftoverStr, mt);
			if(*leftoverStr != NULL && *leftoverStr + 1 == searchLimit &&  *(*leftoverStr + 1) == ' ')
				*leftoverStr = NULL;

			return retPtr;
		}

		char* startPtr = tempPtr[0] + 1;
		char* endPtr = strchr(startPtr, '"');

		if(endPtr == NULL)
			return NULL;

		
	}
}

/* Tokenizes string in a similar way an interactive command line would */
char* plTokenize(char* string, char** leftoverStr, plmt_t* mt){
	if(string == NULL || leftoverStr == NULL || mt == NULL)
		return NULL;

	char* tempPtr[2] = { strchr(string, '"'), strchr(string, ' ') };
	char* searchLimit = string + strlen(string);
	char* startPtr = NULL;
	char* endPtr = NULL;
	char* retPtr;

	if(strlen(string) == 0){
		return NULL;
	}

	/* If string starts with a space and string pointer is lower than the search limit, *\
	\* keep increasing string pointer until it equals tempPtr[1]                        */
	if(*string == ' '){
		while(*string == ' ' && string < searchLimit)
			string++;

		if(tempPtr[1] < string)
			tempPtr[1] = strchr(string, ' ');
	}

	/* If there are no string in quotes, or if the quotes come after a whitespace, assign the   *\
	   beginning of the string to be returned as the pointer of the given string and the end of
	\* the string as the next whitespace                                                        */
	if((!tempPtr[0] && tempPtr[1]) || (tempPtr[1] && tempPtr[1] < tempPtr[0])){
		startPtr = string;
		endPtr = tempPtr[1];
	/* Else, if the beginning of the given string begins with a quote, assign the beginning   *\
	   of the string to be returned as the opening quote + 1 and the end of the string as the
	\* closing quote                                                                          */
	}else if(tempPtr[0] && tempPtr[0] == string){
		startPtr = tempPtr[0] + 1;
		endPtr = strchr(tempPtr[0] + 1, '"');
	}

	size_t strSize = (endPtr - startPtr);

	/* If there's no whitespace or quotes in the given string, return the given string. */
	if(!startPtr || !endPtr || !strSize){
		if(strlen(string) != 0){
			strSize = strlen(string);
			*leftoverStr = NULL;
			startPtr = string;
		}else{
			return NULL;
		}
	}else{
	/* Else what remains of the given string into the leftover variable */
		*leftoverStr = endPtr+1;
	}

	/* Allocate memory for return string, copy the selected part of the string, *\
	\* and null-terminate the return string                                     */
	retPtr = plMTAlloc(mt, (strSize + 1) * sizeof(char));
	memcpy(retPtr, startPtr, strSize);

	retPtr[strSize] = '\0';
	return retPtr;
}

/* Parses a string into an array */
plarray_t* plParser(char* input, plmt_t* mt){
	if(!input || !mt)
		return NULL;

	char* leftoverStr;
	plarray_t* returnStruct = plMTAlloc(mt, sizeof(plarray_t));
	returnStruct->size = 1;
	returnStruct->array = plMTAlloc(mt, 2 * sizeof(char*));

	/* First token */
	char* tempPtr = plTokenize(input, &leftoverStr, mt);
	((char**)returnStruct->array)[0] = tempPtr;

	/* Keep tokenizing until there is no more string left to tokenize */
	while((tempPtr = plTokenize(leftoverStr, &leftoverStr, mt)) != NULL){
		returnStruct->size++;
		char** tempArrPtr = plMTRealloc(mt, returnStruct->array, returnStruct->size * sizeof(char*));

		if(!tempArrPtr){
			for(int i = 0; i < returnStruct->size; i++)
				plMTFree(mt, ((char**)returnStruct->array)[i]);

			plMTFree(mt, returnStruct->array);
			plMTFree(mt, returnStruct);

			return NULL;
		}

		returnStruct->array = tempArrPtr;
		((char**)returnStruct->array)[returnStruct->size - 1] = tempPtr;
	}

	returnStruct->isMemAlloc = true;
	returnStruct->mt = mt;

	return returnStruct;
}

