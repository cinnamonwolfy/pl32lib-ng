/*****************************************************************\
 pl32lib-ng, v1.00
 (c) 2022 pocketlinux32, Under MPL v2.0
 pl32-token.c: String manipulation and parser module
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
	memcpy(retPtr, str, strSize);
	retPtr[strSize] = '\0';

	if(endPtr != searchLimit)
		*leftoverStr = endPtr + delimSize;
	else
		*leftoverStr = NULL;

	return retPtr;
}

/* Tokenizes a string similarly to how it is done in a shell interpreter */
char* plTokenize(char* string, char** leftoverStr, plmt_t* mt){
	if(string == NULL || leftoverStr == NULL || mt == NULL)
		return NULL;

	if(strlen(string) == 0)
		return NULL;

	char* tempPtr[2] = { strchr(string, '"'), strchr(string, '\'') };
	char* spaceChar = strchr(string, ' ');

	/* String checks */
	bool noQuotesFound = (tempPtr[0] == NULL && tempPtr[1] == NULL);
	bool noEndQuotesFound = (tempPtr[0] != NULL && strchr(tempPtr[0] + 1, '"') == NULL) || (tempPtr[1] != NULL && strchr(tempPtr[1] + 1, '\'') == NULL);
	bool spaceComesFirst = (tempPtr[0] == NULL || spaceChar < tempPtr[0]) && (tempPtr[1] == NULL || spaceChar < tempPtr[1]);
	bool literalBeforeBasicStr = (tempPtr[1] != NULL && tempPtr[1] < tempPtr[0]);

	/* If there are no quotes or there are no end quotes or space comes before any quote symbols, *\
	\* use strtok to get a token surrounded by whitespace                                         */
	if(noQuotesFound || noEndQuotesFound || (spaceChar != NULL && spaceComesFirst)){
		return plStrtok(string, " ", leftoverStr, mt);
	}else{
		char* retPtr = NULL;
		char* searchLimit = string + strlen(string);

		/* If a literal string started before a basic one, tokenize using plStrtok */
		if(literalBeforeBasicStr && strchr(tempPtr[1] + 1, '\'') != NULL){
			retPtr = plStrtok(tempPtr[1] + 1, "'", leftoverStr, mt);
			if(*leftoverStr != NULL && *leftoverStr + 1 == searchLimit)
				*leftoverStr = NULL;

			return retPtr;
		}

		/* If none of the above is true, start tokenizing a basic string */
		char* startPtr = tempPtr[0] + 1;
		char* endPtr = strchr(startPtr, '"');

		/* If the end quote is escaped, keep searching for an end quote */
		while(endPtr != NULL && *(endPtr - 1) == '\\')
			endPtr = strchr(endPtr + 1, '"');

		/* If an end quote has not been found, return a NULL pointer */
		if(endPtr == NULL){
			*leftoverStr = NULL;
			return NULL;
		}

		/* Copy the basic string into a memory-allocated buffer */
		size_t strSize = endPtr - startPtr;
		retPtr = plMTAlloc(mt, strSize + 1);
		memcpy(retPtr, startPtr, strSize);
		retPtr[strSize] = '\0';

		char* holderPtr;
		while((holderPtr = strchr(retPtr, '\\')) != NULL){
			memcpy(holderPtr - 1, holderPtr + 1, strlen(holderPtr + 1));
			holderPtr++;
		}

		/* If the end quote is one char away from the end of the input string, *\
		\* set *leftoverStr as NULL. Otherwise, set *leftoverStr as endPtr + 1 */
		if(endPtr + 1 == searchLimit){
			*leftoverStr = NULL;
		}else{
			*leftoverStr = endPtr + 1;
		}

		return retPtr;
	}
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

