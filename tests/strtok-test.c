#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* tokenizeStringWithQuotes(char* string, char** leftoverStr){
	char* retVar;
	char* startVar = strchr(string, '\"');
	char* endVar;

	if(!startVar)
		return NULL;

	endVar = strchr(startVar, '\"');

	if(!endVar)
		return NULL

	size_t strSize = (endVar - startVar) + 1;

	retVar = malloc(strSize * sizeof(char));

	memcpy(retVar, startVar, strSize - 1);

	*leftoverStr = endVar;

	
}

int main(){
	char nano[128] = "printf \"this is a test sample text\" \"nano\"";
	char* charHead;
	char* garbage;

	char* nanoRet = tokenizeStringWithQuotes(nano, &charHead);
	printf("String 1:");
	free(nanoRet);
	nanoRet = tokenizeStringWithQuotes(charHead, &garbage);
	printf("String 2: %s\n", nanoRet);
}
