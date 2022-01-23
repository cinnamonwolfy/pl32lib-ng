/********************************************\
* pl32lib, v0.01                             *
* (c)2021 pocketlinux32, Under Lesser GPLv3  *
* Memory Management/Garbage Collector module *
\********************************************/
#include <pl32-memory.h>

// Internal type for representing internal pointer references
typedef struct plpointer {
	void* pointer;
	size_t size;
} plptr_t

// Structure of the garbage collector. This garbage collector is thread-specific
struct plgc {
	plptr_t* usedPointers;
	plptr_t* freedPointers;
	size_t upAmnt;
	size_t fpAmnt;
	size_t usedMemory;
	size_t maxMemory;
	void* start;
	bool isInitialized;
};

// Controller for garbage collector
int plGCManage(plgc_t* gc, int mode, void* ptr, size_t size){
	if(gc == NULL){
		return 1;
	}

	switch(mode){
		// Initializes the garbage collector
		case PLGC_INIT:
			if(!gc->isInitialized){
				gc->upAmnt = 0;
				gc->fpAmnt = 0;
				gc->usedMemory = 0;
				gc->maxMemory = 128 * 1024 * 1024;
				gc->start = ptr;
				gc->isInitialized = true;
			}
			break;
		// Manually makes the garbage collector clean up memory
		case PLGC_CLEAN:
			break;
		// Cleans memory for the last time and deallocates the garbage collector
		case PLGC_STOP:
			break;
		// Adds pointer reference to the tracking array
		case PLGC_ADDPTR:
			break;
		// Removes pointer reference from the tracking array
		case PLGC_RMPTR:
			break;
	}
}

// malloc() wrapper that interfaces with the garbage collector
void* plGCAlloc(plgc_t* gc, size_t size){
	void* tempPtr;

	if((tempPtr = malloc(size)) == NULL){
		return NULL;
	}

	plGCManage(gc, PLGC_ADDPTR, tempPtr, size);
}

// calloc() wrapper that interfaces with the garbage collector
void* plGCCalloc(plgc_t* gc, size_t amount, size_t size){
	void* tempPtr;

	if((tempPtr = calloc(amount, size)) == NULL){
		return NULL;
	}

	plGCManage(gc, PLGC_ADDPTR, tempPtr, size);
}

// realloc() wrapper that interfaces with the garbage collector
void* plGCRealloc(plgc_t* gc, void* pointer, size_t size){
	void* tempPtr = realloc(pointer, size);

	if((tempPtr = realloc(pointer, size)) == NULL){
		return NULL;
	}

	plGCManage(gc, PLGC_RMPTR, tempPtr, size);
	plGCManage(gc, PLGC_ADDPTR, tempPtr, size);
}

// free() wrapper that interfaces with the garbage collector
void plGCFree(plgc_t* gc, void* pointer){
	free(pointer);
	plGCManage(gc, PLGC_RMPTR, tempPtr, 0);
}
