/****************************************\
* pl32-test: pl32lib testcase            *
* (c)2022 pocketlinux32, Under MPL v2.0  *
\****************************************/
#include <pl32-memory.h>
#include <pl32-file.h>
#include <pl32-token.h>
bool nonInteractive = false;

void printArray(int* array, size_t size){
	printf("Printing out array:\n");
	for(int i = 0; i < size; i++){
		printf("	array[%d]: %d\n", i, array[i]);
	}
}

void printCurrentMemUsg(plmt_t* mt){
	char gchVar;

	printf("Current RAM usage: %ld bytes\n", plMTMemAmnt(mt, PLMT_GET_USEDMEM, 0));
	printf("Press Enter to continue test...");
	if(!nonInteractive)
		gchVar = getchar();
}

int testLoop(char* strToTokenize, plmt_t* mt){
	char* holder;
	char* result = plTokenizeStrtok(strToTokenize, &holder, mt);
	int i = 2;

	if(result == NULL)
		return 1;

	printf("Token 1: %s\n", result);
	plMTFree(mt, result);
	while((result = plTokenizeStrtok(holder, &holder, mt)) != NULL){
		printf("Token %d: %s\n", i, result);
		plMTFree(mt, result);
		i++;
	}

	return 0;
}

int plMemoryTest(/*plarray_t* args, */plmt_t* mt){
	printCurrentMemUsg(mt);

	printf("Allocating and initializing int array (4 ints)...");

	int* nano = plMTAlloc(mt, sizeof(int) * 4);
	for(int i = 0; i < 4; i++){
		nano[i] = i + 1;
	}

	printf("Done.\n");

	printArray(nano, 4);
	printCurrentMemUsg(mt);

	printf("Reallocating int array...");

	void* tempPtr = plMTRealloc(mt, nano, sizeof(int) * 8);
	if(tempPtr == NULL){
		printf("Error!\n Error: plMTRealloc() returned NULL\n");
		return 1;
	}else{
		nano = tempPtr;
		printf("Done\n");
		printCurrentMemUsg(mt);

		printf("Deallocating int array...");
		plMTFree(mt, nano);
		printf("Done\n");
		printCurrentMemUsg(mt);

		printf("Testing double free protection...");
		plMTFree(mt, nano);
		printf("Done\n");

		printf("Allocating multiple arrays of multiple sizes...");

		char* nano2 = plMTAlloc(mt, sizeof(char) * 16);
		char** nano3 = plMTAlloc(mt, sizeof(char*) * 4);
		int* nano4 = plMTAlloc(mt, sizeof(int) * 10);
		int* nano5 = plMTAlloc(mt, sizeof(int) * 20);

		printf("Done\n");
		printCurrentMemUsg(mt);

		printf("Deallocating arrays...");

		plMTFree(mt, nano2);
		plMTFree(mt, nano3);
		plMTFree(mt, nano4);
		plMTFree(mt, nano5);

		printf("Done\n");
		printCurrentMemUsg(mt);
	}

	return 0;
}

int plFileTest(/*plarray_t* args,*/ plmt_t* mt){
	char stringBuffer[4096] = "";
	char filepath[256] = "src/pl32-file.c";
	/*if(args->size > 1)
		strcpy(filepath, ((char**)args->array)[1]);*/

	printf("Opening an existing file...");
	plfile_t* realFile = plFOpen(filepath, "r", mt);
	plfile_t* memFile = plFOpen(NULL, "w+", mt);

	if(realFile == NULL){
		printf("Error!\nError opening file. Exiting...\n");
		plFClose(memFile);
		return 1;
	}

	printf("Done\n");
	printf("Contents of %s:\n\n", filepath);
	while(plFGets(stringBuffer, 4095, realFile) != NULL){
		printf("%s", stringBuffer);
		for(int i = 0; i < 4096; i++)
			stringBuffer[i] = 0;
	}

	printf("Reading and writing to file-in-memory...");
	plFPuts("test string getting sent to the yes\nnano", memFile);
	plFSeek(memFile, 0, SEEK_SET);
	printf("Done\n");
	printf("Contents of file-in-memory:\n");
	while(plFGets(stringBuffer, 4095, memFile) != NULL){
		printf("%s", stringBuffer);
		for(int i = 0; i < 4096; i++)
			stringBuffer[i] = 0;
	}

	plFClose(realFile);
	plFClose(memFile);

	return 0;
}

int plShellTest(/*plarray_t* args, */plmt_t* mt){
	char* tknTestStrings[6] = { "oneword", "two words", "\"multiple words enclosed by quotes\" not anymore x3", "\"quotes at the beginning\" some stuff in the middle \"and now quotes at the back\"", "\"just quotes x3\"", "\'time for a literal string :3\' with stuff \"mixed all over\" it x3" };

	printf("This is a test of the pl32-shell tokenizer\n\n");

	for(int i = 0; i < 6; i++){
		printf("Test %d:\n", i);
		if(testLoop(tknTestStrings[i], mt)){
			printf("An error occurred. Exiting...\n");
			return 1;
		}
	}

	return 0;
}

int main(int argc, const char* argv[]){
	plmt_t* mainMT = plMTInit(8 * 1024 * 1024);
/*	plarray_t commandBuf, variableBuf;

	int plmajorver = 4;
	int plminorver = 0;
	int birthyear = 2020;
	char string[16] = "Hello, World!";

	commandBuf.array = plMTAlloc(mainMT, sizeof(plfunctionptr_t) * 4);
	commandBuf.size = 4;
	commandBuf.isMemAlloc = true;
	variableBuf.array = plMTAlloc(mainMT, sizeof(plvariable_t) * 4);
	variableBuf.size = 4;
	variableBuf.isMemAlloc = true;

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
	((plvariable_t*)variableBuf.array)[3].type = PLSHVAR_STRING;
	((plvariable_t*)variableBuf.array)[3].name = "test_string";
	((plvariable_t*)variableBuf.array)[3].isMemAlloc = false;

	plShell(argv[1], &variableBuf, &commandBuf, &mainMT);*/

	if(argc < 2)
		return 1;

	if(argc > 2)
		nonInteractive = true;

	if(strcmp(argv[1], "parser-test") == 0){
		plShellTest(mainMT);
	}else if(strcmp(argv[1], "memory-test") == 0){
		plMemoryTest(mainMT);
	}else if(strcmp(argv[1], "file-test") == 0){
		plFileTest(mainMT);
	}else{
		return 1;
	}
}
