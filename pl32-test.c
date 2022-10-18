/******************************************************\
* pl32-test: pl32lib testcase                          *
* (c)2022 pocketlinux32, Under Lesser GPLv2.1 or later *
\******************************************************/
#include <pl32-file.h>
#include <pl32-term.h>

void printArray(int* array, size_t size){
	printf("Printing out array:\n");
	for(int i = 0; i < size; i++){
		printf("	array[%d]: %d\n", i, array[i]);
	}
}

void printCurrentMemUsg(plgc_t* gc){
	char gchVar;

	printf("Current RAM usage: %ld bytes\n", plGCMemAmnt(gc, PLGC_GET_USEDMEM, 0));
	printf("Press Enter to continue test...");
	gchVar = getchar();
}

int testLoop(char* strToTokenize, plgc_t* gc){
	char* holder;
	char* result = plTokenize(strToTokenize, &holder, gc);
	int i = 2;

	if(!result)
		return 1;

	printf("Token 1: %s\n", result);
	plGCFree(gc, result);
	while((result = plTokenize(holder, &holder, gc)) != NULL){
		printf("Token %d: %s\n", i, result);
		plGCFree(gc, result);
		i++;
	}
}

int plMemoryTest(plarray_t* args, plgc_t* gc){
	printCurrentMemUsg(gc);

	printf("Allocating and initializing int array (4 ints)...");

	int* nano = plGCAlloc(gc, sizeof(int) * 4);
	for(int i = 0; i < 4; i++){
		nano[i] = i + 1;
	}

	printf("Done.\n");

	printArray(nano, 4);
	printCurrentMemUsg(gc);

	printf("Reallocating int array...");

	void* tempPtr = plGCRealloc(gc, nano, sizeof(int) * 8);
	if(!tempPtr){
		printf("Error!\n Error: plGCRealloc() returned NULL\n");
	}else{
		nano = tempPtr;
		printf("Done\n");
		printCurrentMemUsg(gc);

		printf("Deallocating int array...");
		plGCFree(gc, nano);
		printf("Done\n");
		printCurrentMemUsg(gc);

		printf("Testing double free protection...");
		plGCFree(gc, nano);
		printf("Done\n");

		printf("Allocating multiple arrays of multiple sizes...");

		char* nano2 = plGCAlloc(gc, sizeof(char) * 16);
		char** nano3 = plGCAlloc(gc, sizeof(char*) * 4);
		int* nano4 = plGCAlloc(gc, sizeof(int) * 10);
		int* nano5 = plGCAlloc(gc, sizeof(int) * 20);

		printf("Done\n");
		printCurrentMemUsg(gc);

		printf("Deallocating arrays...");

		plGCFree(gc, nano2);
		plGCFree(gc, nano3);
		plGCFree(gc, nano4);
		plGCFree(gc, nano5);

		printf("Done\n");
		printCurrentMemUsg(gc);
	}

	return 0;
}

int plFileTest(plarray_t* args, plgc_t* gc){
	char stringBuffer[4096] = "";

	printf("Opening an existing file...");
	plfile_t* realFile = plFOpen("include/pl32-file.h", "r", gc);
	plfile_t* memFile = plFOpen(NULL, "w+", gc);

	if(!realFile){
		printf("Error opening file. Exiting...\n");
	}

	printf("Done\n");
	printf("Contents of include/pl32-file.h:\n\n");
	while(plFGets(stringBuffer, 4095, realFile) != NULL){
		printf("%s", stringBuffer);
		for(int i = 0; i < 4096; i++)
			stringBuffer[i] = 0;
	}

	printf("Reading and writing to file-in-memory...");
	plFPuts("test string getting sent to the yes\nnano", memFile);
	plFSeek(memFile, 0, SEEK_SET);
	printf("Done\n");
	printf("Contents of file-in-memory:\n", stringBuffer);
	while(plFGets(stringBuffer, 4095, memFile) != NULL){
		printf("%s", stringBuffer);
		for(int i = 0; i < 4096; i++)
			stringBuffer[i] = 0;
	}

	plFClose(realFile);
	plFClose(memFile);

	return 0;
}

int plShellTest(plarray_t* args, plgc_t* gc){
	char* tknTestStrings[5] = { "oneword", "two words", "\"multiple words enclosed by quotes\" not anymore lol", "\"quotes at the beginning\" some stuff in the middle \"and now quotes at the back\"", "\"just quotes lol\"" };

	printf("This is a test of the pl32-shell tokenizer\n\n");

	for(int i = 0; i < 5; i++){
		printf("Test %d:\n", i);
		if(testLoop(tknTestStrings[i], gc)){
			printf("An error occurred. Exiting...\n");
			return 1;
		}
	}

	return 0;
}

int plTermTest(plarray_t* args, plgc_t* gc){
	if(args->size < 2){
		printf("Error: Not enough args\n");
		printf("Usage: %s [serial-port]\n", ((char**)args->array)[0]);
		return 1;
	}

	plterminal_t* terminal = plOpenTerminal(((char**)args->array)[1], gc);
	if(!terminal)
		return 2;

	plTermRawInit(terminal);
	plTermInteractive(terminal);
	plCloseTerminal(terminal, gc);

	return 0;
}

int main(int argc, const char* argv[]){
	plgc_t* mainGC = plGCInit(8 * 1024 * 1024);
	plarray_t commandBuf, variableBuf;

	int plmajorver = 4;
	int plminorver = 0;
	int birthyear = 2020;
	char string[16] = "Hello, World!";

	commandBuf.array = plGCAlloc(mainGC, sizeof(plfunctionptr_t) * 4);
	commandBuf.size = 4;
	variableBuf.array = plGCAlloc(mainGC, sizeof(plvariable_t) * 4);
	variableBuf.size = 4;

	((plfunctionptr_t*)commandBuf.array)[0].function = plShellTest;
	((plfunctionptr_t*)commandBuf.array)[0].name = "parser-test";
	((plfunctionptr_t*)commandBuf.array)[1].function = plMemoryTest;
	((plfunctionptr_t*)commandBuf.array)[1].name = "memory-test";
	((plfunctionptr_t*)commandBuf.array)[2].function = plFileTest;
	((plfunctionptr_t*)commandBuf.array)[2].name = "file-test";
	((plfunctionptr_t*)commandBuf.array)[3].function = plTermTest;
	((plfunctionptr_t*)commandBuf.array)[3].name = "term-test";

	((plvariable_t*)variableBuf.array)[0].varptr = &plmajorver;
	((plvariable_t*)variableBuf.array)[0].type = PLSHVAR_INT;
	((plvariable_t*)variableBuf.array)[0].name = "plver";
	((plvariable_t*)variableBuf.array)[0].isMemAlloc = false;
	((plvariable_t*)variableBuf.array)[1].varptr = &plminorver;
	((plvariable_t*)variableBuf.array)[1].type = PLSHVAR_INT;
	((plvariable_t*)variableBuf.array)[1].name = "plfeatlvl";
	((plvariable_t*)variableBuf.array)[1].isMemAlloc = false;
	((plvariable_t*)variableBuf.array)[2].varptr = &birthyear;
	((plvariable_t*)variableBuf.array)[2].type = PLSHVAR_INT;
	((plvariable_t*)variableBuf.array)[2].name = "birthyear";
	((plvariable_t*)variableBuf.array)[2].isMemAlloc = false;
	((plvariable_t*)variableBuf.array)[3].varptr = string;
	((plvariable_t*)variableBuf.array)[3].type = PLSHVAR_INT;
	((plvariable_t*)variableBuf.array)[3].name = "test_string";
	((plvariable_t*)variableBuf.array)[3].isMemAlloc = false;

	plShellInteractive(NULL, true, &variableBuf, &commandBuf, mainGC);
}
