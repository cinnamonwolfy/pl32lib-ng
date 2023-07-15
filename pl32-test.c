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
	fflush(stdout);
	if(!nonInteractive)
		gchVar = getchar();
	else
		fputs("\n", stdout);
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
	char stringBuffer[4096] = "";
	char filepath[256] = "src/pl32-file.c";
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
	string_t tknTestStrings[10] = { "oneword", "two words", "\"multiple words enclosed by quotes\" not anymore x3", "\"quotes at the beginning\" some stuff in the middle \"and now quotes at the back\"", "\"just quotes x3\"", "\'time for a literal string :3\' with stuff \"mixed all over\" it x3", "\"\\\"Escaped quotes this time\\\"\" and 'just a literal string with no ending :3", "\"now we have a basic string with no ending but 'a literal that does :3'", "string    with an  embedded = newline \" char\"\n  ", "[\"array test\",\'literal string here\', \"basic string here, preceded by a space\"]" };

	printf("This is a test of the pl32lib-ng tokenizer\n\n");

	for(int i = 0; i < 10; i++){
		printf("Test %d:\n", i);
		if(testLoop(tknTestStrings[i], mt)){
			printf("An error occurred. Exiting...\n");
			return 1;
		}
	}

	return 0;
}

int plUStringTest(plmt_t* mt){
	plstring_t convertedStr = plUStrFromCStr("hewwo wowwd :3", mt);
	plstring_t plCharString = {
		.data = {
			.array = plMTAlloc(mt, 14 * sizeof(plchar_t)),
			.size = 14,
			.isMemAlloc = true,
			.mt = mt
		},
		.isplChar = true
	};

	((plchar_t*)plCharString.data.array)[0].bytes[0] = 'h';
	((plchar_t*)plCharString.data.array)[1].bytes[0] = 'e';
	((plchar_t*)plCharString.data.array)[2].bytes[0] = 'w';
	((plchar_t*)plCharString.data.array)[3].bytes[0] = 'w';
	((plchar_t*)plCharString.data.array)[4].bytes[0] = 'o';
	((plchar_t*)plCharString.data.array)[5].bytes[0] = ' ';
	((plchar_t*)plCharString.data.array)[6].bytes[0] = 'w';
	((plchar_t*)plCharString.data.array)[7].bytes[0] = 'o';
	((plchar_t*)plCharString.data.array)[8].bytes[0] = 'w';
	((plchar_t*)plCharString.data.array)[9].bytes[0] = 'w';
	((plchar_t*)plCharString.data.array)[10].bytes[0] = 'd';
	((plchar_t*)plCharString.data.array)[11].bytes[0] = ' ';
	((plchar_t*)plCharString.data.array)[12].bytes[0] = ':';
	((plchar_t*)plCharString.data.array)[13].bytes[0] = '3';
	plUStrCompress(&plCharString, mt);

	fputs("Converted C String: ", stdout);
	fwrite(convertedStr.data.array, 1, convertedStr.data.size, stdout);
	fputs("\n", stdout);

	fputs("Compressed plchar_t string: ", stdout);
	fwrite(plCharString.data.array, 1, plCharString.data.size, stdout);
	fputs("\n\n", stdout);

	plstring_t matchStr = plUStrFromCStr("wo", NULL);
	plchar_t plChr = {
		.bytes = { 'w', '\0', '\0', '\0'  }
	};

	puts("plchar_t-based Matching Test");

	int64_t retIndex = plUStrchr(&convertedStr, plChr, 0);
	if(retIndex == -1)
		plPanic("main: Offset is negative!", false, true);

	printf("Returned Offset: %ld\n", retIndex);
	printf("Current Value: %c\n\n", *((char*)convertedStr.data.array + retIndex));

	puts("plstring_t-based Matching Test");

	retIndex = plUStrstr(&convertedStr, &matchStr, 5);
	if(retIndex == -1)
		plPanic("main: Offset is negative!", false, true);

	printf("Returned Offset: %ld\n", retIndex);
	fputs("Current Value: ", stdout);
	fwrite(convertedStr.data.array + retIndex, 1, 8, stdout);
	fputs("\n\n", stdout);

	plChr.bytes[0] = ' ';
	plstring_t holderStr = {
		.data = {
			.array = NULL,
			.size = 0,
			.isMemAlloc = false,
			.mt = NULL
		},
		.isplChar = false
	};
	plstring_t leftoverStr;
	plstring_t delimiterArr = {
		.data = {
			.array = &plChr,
			.size = 1,
			.isMemAlloc = false,
			.mt = NULL
		},
		.isplChar = true
	};
	plstring_t tokenizedStr = plUStrtok(&convertedStr, &delimiterArr, &holderStr, mt);
	memcpy(&leftoverStr, &holderStr, sizeof(plstring_t));

	puts("plstring_t-based String Tokenizer Test");

	if(tokenizedStr.data.array == NULL)
		plPanic("main: Token is NULL!", false, true);

	fputs("Current Value: ", stdout);
	fwrite(tokenizedStr.data.array, 1, tokenizedStr.data.size, stdout);
	fputs("\n", stdout);

	for(int i = 0; i < 2; i++){
		tokenizedStr = plUStrtok(&leftoverStr, &delimiterArr, &holderStr, mt);
		memcpy(&leftoverStr, &holderStr, sizeof(plstring_t));
		fputs("Current Value: ", stdout);
		fwrite(tokenizedStr.data.array, 1, tokenizedStr.data.size, stdout);
		fputs("\n", stdout);
	}

	return 0;
}

int main(int argc, string_t argv[]){
	plmt_t* mainMT = plMTInit(8 * 1024 * 1024);

	if(argc < 2){
		printf("Valid test values:\n parser-test\n memory-test\n file-test\n ustring-test\n");
		return 1;
	}

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
	}else if(strcmp(argv[1], "ustring-test") == 0){
		return plUStringTest(mainMT);
	}else{
		return 1;
	}

	plMTStop(mainMT);
}
