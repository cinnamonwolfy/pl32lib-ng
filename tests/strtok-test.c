#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char* tokenizeString(char* string, char** leftoverStr){
	char* retVar;
	char* startVar1 = strchr(string, '"');
	char* startVar2 = strchr(string, ' ');
	char* endVar;
	char* searchLimit = string + strlen(string);
	size_t strSize;

	if(!startVar1 && startVar2 && startVar2 > string + 1){
		strSize = (string - startVar2);
	}else if(startVar1){
		endVar = strchr(startVar1+1, '"');
		strSize = (endVar - startVar1);
	}else{
		return NULL;
	}

	if(!endVar)
		return NULL;

	printf("%ld\n", strSize);

	retVar = malloc(strSize * sizeof(char));
	memcpy(retVar, startVar1 + 1, strSize - 1);

	int i = 0;
	while(*(endVar + i) == ' ' && endVar + i < searchLimit){
		i++;
	}

	*leftoverStr = endVar+1;

	return retVar;
}

int main(){
	char nano[128] = "printf \"this is a test sample text\" not nano \"nano\"";
	char* charHead;
	char* garbage;
	int i = 2;

	char* nanoRet = tokenizeString(nano, &charHead);

	if(!nanoRet)
		return -1;

	printf("String 1: %s\n", nanoRet);
	free(nanoRet);
	while((nanoRet = tokenizeString(charHead, &charHead)) != NULL){
		printf("String %d: %s\n", i, nanoRet);
		free(nanoRet);
		i++;
	}

	return 0;
}
