/*****************************************************************\
 pl32lib-ng, v1.01
 (c) 2022 pocketlinux32, Under MPL v2.0
 pl32-token.c: String manipulation and parser module
\*****************************************************************/
#include <pl32-token.h>

/* A thread-safe reimplementation of Standard C function strtok */
string_t plStrtok(string_t str, string_t delim, string_t* leftoverStr, plmt_t* mt){
	if(str == NULL || delim == NULL || leftoverStr == NULL || mt == NULL)
		return NULL;

	size_t delimSize = strlen(delim);
	int iterator = 0;

	string_t retPtr;
	string_t endPtr = NULL;
	string_t searchLimit = str + strlen(str);

	while(endPtr == NULL && str < searchLimit){
		char* tempChr = strchr(str, delim[iterator]);

		if(tempChr == NULL){
			iterator++;
			if(iterator >= delimSize)
				endPtr = searchLimit;
		}else{
			for(int i = 0; i < delimSize; i++){
				if(delim[i] != delim[iterator]){
					char* tempChr2 = strchr(str, delim[i]);
					if(tempChr2 != NULL && tempChr2 < tempChr)
						tempChr = tempChr2;
				}
			}

			if(tempChr == str){
				str++;
				iterator = 0;
			}else{
				endPtr = tempChr;
			}
		}

	}

	if(endPtr == NULL){
		*leftoverStr = NULL;
		return NULL;
	}

	/* Copies the memory block into the return pointer */
	size_t strSize = endPtr - str;
	retPtr = plMTAllocE(mt, strSize + 1);
	memcpy(retPtr, str, strSize);
	retPtr[strSize] = '\0';

	/* Calculates the pointer to put into leftoverStr*/
	*leftoverStr = NULL;
	if(endPtr != searchLimit){
		char* strPtr = endPtr + 1;
		iterator = 0;
		while(*leftoverStr == NULL && strPtr < searchLimit){
			char* tempChr = strchr(strPtr, delim[iterator]);

			if(tempChr == NULL || tempChr > strPtr + 1){
				iterator++;
				if(iterator >= delimSize)
					*leftoverStr = strPtr;
			}else{
				strPtr++;
				if(tempChr == strPtr)
					iterator = 0;
			}
		}
	}

	return retPtr;
}

/* Tokenizes a string similarly to how it is done in a shell interpreter */
string_t plTokenize(string_t string, string_t* leftoverStr, plmt_t* mt){
	if(string == NULL || leftoverStr == NULL || mt == NULL)
		return NULL;

	if(strlen(string) == 0)
		return NULL;

	string_t tempPtr[2] = { strchr(string, '"'), strchr(string, '\'') };
	string_t spaceChar = strchr(string, ' ');

	/* String checks */
	bool noQuotesFound = (tempPtr[0] == NULL && tempPtr[1] == NULL);
	bool noEndQuoteBasic = (tempPtr[0] != NULL && strchr(tempPtr[0] + 1, '"') == NULL);
	bool noEndQuoteLiteral = (tempPtr[1] != NULL && strchr(tempPtr[1] + 1, '\'') == NULL);
	bool spaceComesFirst = (tempPtr[0] == NULL || spaceChar < tempPtr[0]) && (tempPtr[1] == NULL || spaceChar < tempPtr[1]);
	bool literalBeforeBasicStr = (tempPtr[1] != NULL && (tempPtr[0] == NULL || tempPtr[1] < tempPtr[0]));

	/* If there are no quotes or there are no end quotes or space comes before any quote symbols, *\
	\* use strtok to get a token surrounded by whitespace                                         */
	if(noQuotesFound || (noEndQuoteBasic && !literalBeforeBasicStr) || (noEndQuoteLiteral && literalBeforeBasicStr) || (spaceChar != NULL && spaceComesFirst)){
		return plStrtok(string, " \n", leftoverStr, mt);
	}else{
		string_t retPtr = NULL;
		string_t searchLimit = string + strlen(string);

		/* If a literal string started before a basic one, tokenize using plStrtok */
		if(literalBeforeBasicStr && strchr(tempPtr[1] + 1, '\'') != NULL){
			retPtr = plStrtok(tempPtr[1] + 1, "'", leftoverStr, mt);
			if(*leftoverStr != NULL && *leftoverStr + 1 == searchLimit)
				*leftoverStr = NULL;

			return retPtr;
		}

		/* If none of the above is true, start tokenizing a basic string */
		string_t startPtr = tempPtr[0] + 1;
		string_t endPtr = strchr(startPtr, '"');

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
		retPtr = plMTAllocE(mt, strSize + 1);
		memcpy(retPtr, startPtr, strSize);
		retPtr[strSize] = '\0';

		string_t holderPtr = strchr(retPtr, '\\');
		size_t sizeReducer = 0;
		while(holderPtr != NULL){
			memcpy(holderPtr, holderPtr + 1, strlen(holderPtr + 1));
			holderPtr++;
			sizeReducer++;
			holderPtr = strchr(holderPtr, '\\');
		}

		if(sizeReducer != 0){
			void* tempPtr = plMTRealloc(mt, retPtr, strSize + 1 - sizeReducer);
			if(tempPtr == NULL){
				free(retPtr);
				*leftoverStr = NULL;
				return NULL;
			}

			retPtr = tempPtr;
			retPtr[strSize - sizeReducer] = '\0';
		}

		/* If the end quote is one char away from the end of the input string, *\
		   or if end quote is two chars away from the end of the input string
		   and the char is a space or a newline, set *leftoverStr as NULL.
		\* Otherwise, set *leftoverStr as endPtr + 1                           */
		if(endPtr + 1 == searchLimit || (endPtr + 1 == searchLimit - 1 && (*(endPtr + 1) == ' ' || *(endPtr + 1) == '\n'))){
			*leftoverStr = NULL;
		}else{
			*leftoverStr = endPtr + 1;
		}

		return retPtr;
	}
}

/* Parses a string into an array */
plarray_t* plParser(string_t input, plmt_t* mt){
	if(!input || !mt)
		return NULL;

	string_t leftoverStr;
	plarray_t* returnStruct = plMTAllocE(mt, sizeof(plarray_t));
	returnStruct->size = 1;
	returnStruct->array = plMTAllocE(mt, 2 * sizeof(string_t));

	/* First token */
	string_t tempPtr = plTokenize(input, &leftoverStr, mt);
	((string_t*)returnStruct->array)[0] = tempPtr;

	/* Keep tokenizing until there is no more string left to tokenize */
	while((tempPtr = plTokenize(leftoverStr, &leftoverStr, mt)) != NULL){
		returnStruct->size++;
		string_t* tempArrPtr = plMTRealloc(mt, returnStruct->array, returnStruct->size * sizeof(string_t));

		if(!tempArrPtr){
			for(int i = 0; i < returnStruct->size; i++)
				plMTFree(mt, ((string_t*)returnStruct->array)[i]);

			plMTFree(mt, returnStruct->array);
			plMTFree(mt, returnStruct);

			return NULL;
		}

		returnStruct->array = tempArrPtr;
		((string_t*)returnStruct->array)[returnStruct->size - 1] = tempPtr;
	}

	returnStruct->isMemAlloc = true;
	returnStruct->mt = mt;

	return returnStruct;
}

