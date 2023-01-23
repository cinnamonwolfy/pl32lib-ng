/****************************************\
* pl32-test: pl32lib testcase            *
* (c)2022 pocketlinux32, Under MPL v2.0  *
\****************************************/
#include <pl32.h>

bool nonInteractive = false;

void printArray(int* array, size_t size){
	printf("Printing out array:\n");
	for(int i = 0; i < size; i++){
		printf("	array[%d]: %d\n", i, array[i]);
	}
}

void printCurrentMemUsg(plmt_t* mt){
	byte_t gchVar;

	printf("Current RAM usage: %ld bytes\n", plMTMemAmnt(mt, PLMT_GET_USEDMEM, 0));
	printf("Press Enter to continue test...");
	if(!nonInteractive)
		gchVar = getchar();
}

int testLoop(string_t strToTokenize, plmt_t* mt){
	string_t holder;
	string_t result = plTokenize(strToTokenize, &holder, mt);
	int i = 2;

	if(result == NULL)
		return 1;

	printf("Token 1: %s\n", result);
	plMTFree(mt, result);
	while((result = plTokenize(holder, &holder, mt)) != NULL){
		printf("Token %d: %s\n", i, result);
		plMTFree(mt, result);
		i++;
	}

	return 0;
}

int plMemoryTest(plmt_t* mt){
	printCurrentMemUsg(mt);

	printf("Allocating and initializing int array (4 ints)...");

	int* nano = plMTAllocE(mt, sizeof(int) * 4);
	for(int i = 0; i < 4; i++){
		nano[i] = i + 1;
	}

	printf("Done.\n");

	printArray(nano, 4);
	printCurrentMemUsg(mt);

	printf("Reallocating int array...");

	memptr_t tempPtr = plMTRealloc(mt, nano, sizeof(int) * 8);
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

	string_t nano2 = plMTAllocE(mt, sizeof(byte_t) * 16);
	string_t* nano3 = plMTAllocE(mt, sizeof(string_t) * 4);
	int* nano4 = plMTAllocE(mt, sizeof(int) * 10);
	int* nano5 = plMTAllocE(mt, sizeof(int) * 20);

	printf("Done\n");
	printCurrentMemUsg(mt);

	printf("Deallocating arrays...");

	plMTFree(mt, nano2);
	plMTFree(mt, nano3);
	plMTFree(mt, nano4);
	plMTFree(mt, nano5);

	printf("Done\n");
	printCurrentMemUsg(mt);

	return 0;
}

int plFileTest(string_t customFile, plmt_t* mt){
	byte_t stringBuffer[4096] = "";
	byte_t filepath[256] = "src/pl32-file.c";
	if(customFile != NULL)
		strcpy(filepath, customFile);

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

int plTokenTest(plmt_t* mt){
	string_t tknTestStrings[9] = { "oneword", "two words", "\"multiple words enclosed by quotes\" not anymore x3", "\"quotes at the beginning\" some stuff in the middle \"and now quotes at the back\"", "\"just quotes x3\"", "\'time for a literal string :3\' with stuff \"mixed all over\" it x3", "\"\\\"Escaped quotes this time\\\"\" and 'just a literal string with no ending :3", "\"now we have a basic string with no ending but 'a literal that does :3'", "string    with an  embedded = newline \"char\"\n  " };

	printf("This is a test of the pl32lib-ng tokenizer\n\n");

	for(int i = 0; i < 9; i++){
		printf("Test %d:\n", i);
		if(testLoop(tknTestStrings[i], mt)){
			printf("An error occurred. Exiting...\n");
			return 1;
		}
	}

	return 0;
}

int main(int argc, string_t argv[]){
	plmt_t* mainMT = plMTInit(8 * 1024 * 1024);

	if(argc < 2)
		return 1;

	if(argc > 2)
		nonInteractive = true;

	if(strcmp(argv[1], "parser-test") == 0){
		return plTokenTest(mainMT);
	}else if(strcmp(argv[1], "memory-test") == 0){
		return plMemoryTest(mainMT);
	}else if(strcmp(argv[1], "file-test") == 0){
		if(argc > 2)
			return plFileTest(argv[2], mainMT);

		return plFileTest(NULL, mainMT);
	}else{
		return 1;
	}
}
