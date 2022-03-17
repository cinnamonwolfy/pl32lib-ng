#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char* tokenizeString(char* string, char** leftoverStr){
	if(string == NULL){
		return NULL;
	}

	char* tempVar[2] = { strchr(string, '"'), strchr(string, ' ') };
	char* searchLimit = string + strlen(string);
	char* startVar = NULL;
	char* endVar = NULL;
	char* retVar;

	if(strlen(string) == 0){
		return NULL;
	}

	if(*string == ' '){
		while(*string == ' ' && string < searchLimit) string++;
		if(tempVar[1] < string){
			tempVar[1] = strchr(string, ' ');
		}
	}

	if((!tempVar[0] && tempVar[1]) || (tempVar[1] && tempVar[1] < tempVar[0])){
		startVar = string;
		endVar = tempVar[1];
	}else if(tempVar[0]){
		startVar = tempVar[0] + 1;
		endVar = strchr(tempVar[0] + 1, '"');
	}

	size_t strSize = (endVar - startVar);
	printf("%ld\n", strSize);

	if(!startVar || !endVar || !strSize){
		if(strlen(string) != 0){
			retVar = malloc(strlen(string) + 1 * sizeof(char));
			memcpy(retVar, string, strlen(string));
			*leftoverStr = NULL;
		}else{
			return NULL;
		}
	}else{
		retVar = malloc(strSize * sizeof(char));
		memcpy(retVar, startVar, strSize);

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
	while((nanoRet = tokenizeString(charHead, &charHead)) != NULL){
		printf("String %d: %s\n", i, nanoRet);
		free(nanoRet);
		i++;
	}

	return 0;
}
