#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char* tokenizeString(char* string, char** leftoverStr){
	char* tempVar[2] = { strchr(string, '"'), strchr(string, ' ') };
	char* startVar;
	char* endVar;
	char* retVar;

	if(){
		
	}

	endVar = strchr(startVar+1, '"');

	if(!startVar || !endVar){
		if(strlen(string) != 0){
			retVar = malloc(strlen(string) + 1 * sizeof(char));
			memcpy(retVar, string, strlen(string));
			*leftoverStr = NULL;
		}else{
			return NULL;
		}
	}else{
		size_t strSize = (endVar - startVar);
		printf("%ld\n", strSize);

		retVar = malloc(strSize * sizeof(char));
		memcpy(retVar, startVar + 1, strSize - 1);

		*leftoverStr = endVar+1;
	}
	return retVar;
}

int main(){
	char nano[128];
	char* charHead;
	//char* garbage;
	int i = 2;

	printf("Give a string (up to 128 chars): ");
	scanf("%128[^\n]", nano);
	getchar();

	char* nanoRet = tokenizeString(nano, &charHead);

	if(!nanoRet)
		return -1;

	printf("String 1: %s\n", nanoRet);
	free(nanoRet);
	while(charHead != NULL && nanoRet != NULL){
		nanoRet = tokenizeString(charHead, &charHead);
		printf("String %d: %s\n", i, nanoRet);
		free(nanoRet);
		i++;
	}

	return 0;
}
