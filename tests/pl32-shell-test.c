#include <pl32-shell.h>

int testLoop(char* strToTokenize, plgc_t* gc){
	char* holder;
	char* result = plTokenize(strToTokenize, &holder, gc);
	int i = 2;

	if(!result)
		return 1;

	printf("Token 1: %s\n", result);
	free(result);
	while((result = plTokenize(holder, &holder, gc)) != NULL){
		printf("Token %d: %s\n", i, result);
		free(result);
		i++;
	}
}

int plShellTest(plarray_t* args, plgc_t* gc){
	char* holder;
	char* token;
	char* testStrings[5] = { "oneword", "two words", "\"multiple words enclosed by quotes\" not anymore lol", "\"quotes at the beginning\" some stuff in the middle \" and now quotes at the back\"", "\"just quotes lol\"" };

	printf("This is a test of the new pl32-shell tokenizer\n");

	for(int i = 0; i < 5; i++){
		printf("Test %d:\n", i);
		if(testLoop(testStrings[i], gc)){
			printf("An error occurred. Exiting...\n");
			return 1;
		}
	}
}

int main(int argc, const char* argv[]){
	plgc_t* mainGC = plGCInit(8 * 1024 * 1024);
	plarray_t commandBuf;

	commandBuf.array = plGCAlloc(mainGC, sizeof(plfunctionptr_t) * 2);
	commandBuf.size = 1;

	((plfunctionptr_t*)commandBuf.array)[0].function = plShellTest;
	((plfunctionptr_t*)commandBuf.array)[0].name = "parser-test";

	plShellInteractive(NULL, &commandBuf, mainGC);
}
