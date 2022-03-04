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
	plgc_t* mainGC;
	plGCManage(mainGC, PLGC_INIT, NULL, 0);

	printCurrentMemUsg(mainGC);

	printf("Allocating and initializing int array (4 ints)...");

	int* nano = plGCAlloc(mainGC, sizeof(int) * 4);
	for(int i = 0; i < 4; i++){
		nano[i] = i + 1;
	}

	printf("Done.\n");

	printArray(nano, 4);
	printCurrentMemUsg(mainGC);

	printf("Deallocating int array...");

	plGCFree(mainGC, nano);

	printf("Done\n");

	printCurrentMemUsg(mainGC);

	plGCManage(mainGC, PLGC_STOP, NULL, 0);
}
