#include <pl32-shell.h>

char* scanfWithPrompt(char* prompt, plgc_t* gc){
	char string[4096];
	printf("%s", prompt);
	scanf("%4096s", string);

	char* returnString = plGCAlloc(gc, strlen(string)+1);
	strcpy(returnString, string);

	return returnString;
}

int main(int argc, const char* argv[]){
	plgc_t* mainGC = plGCInit(131072);
	plarray_t* moreNano;
	char* nano = scanfWithPrompt("Enter a string (up to 4096 characters): ", mainGC);

	printf("%s\n", nano);

	//moreNano = plParser(nano, mainGC);
	//plPrintTokenizedStr(moreNano);
	plGCManage(mainGC, PLGC_STOP, NULL, 0);
}
