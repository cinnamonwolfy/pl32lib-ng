/*****************************\
* pl32-memory test            *
* (c)2022 pocketlinux32       *
\*****************************/
#include <pl32-memory.h>
#include <stdio.h>

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

int main(int argc, const char* argv[]){
	plgc_t* mainGC = plGCInit(2048);

	printCurrentMemUsg(mainGC);

	printf("Allocating and initializing int array (4 ints)...");

	int* nano = plGCAlloc(mainGC, sizeof(int) * 4);
	char* nano2 = plGCAlloc(mainGC, sizeof(char) * 16);
	char** nano3 = plGCAlloc(mainGC, sizeof(char*) * 4);
	int* nano4 = plGCAlloc(mainGC, sizeof(int) * 10);
	int* nano5 = plGCAlloc(mainGC, sizeof(int) * 20);
	for(int i = 0; i < 4; i++){
		nano[i] = i + 1;
	}

	printf("Done.\n");

	printArray(nano, 4);
	printCurrentMemUsg(mainGC);

	printf("Reallocating int array...");

	void* tempPtr = plGCRealloc(mainGC, nano, sizeof(int) * 8);
	if(!tempPtr){
		printf("Error!\n Error: plGCRealloc() returned NULL\n");
	}else{
		printf("Done\n");
		printCurrentMemUsg(mainGC);

		printf("Deallocating int array...");
		plGCFree(mainGC, nano);
		printf("Done\n");

		printCurrentMemUsg(mainGC);

		printf("Testing double free protection...");
		plGCFree(mainGC, nano);
		printf("Done\n");

		plGCFree(mainGC, nano);
		plGCFree(mainGC, nano2);
		plGCFree(mainGC, nano3);
		plGCFree(mainGC, nano4);
		plGCFree(mainGC, nano5);
	}


	printf("Shutting down garbage collector...");
	plGCManage(mainGC, PLGC_STOP, NULL, 0, NULL);
	printf("Done\n");
}
