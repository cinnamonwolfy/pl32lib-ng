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

// Central
int plGCManage(plgc_t* gc, int mode, void* ptr, size_t size){
	if(gc == NULL){
		return 1;
	}

	switch(mode){
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
		case PLGC_CLEAN:

			break;
		case PLGC_STOP:
			break;
		case PLGC_ADDPTR:
			break;
		case PLGC_RMPTR:
			break;
	}
}

void* plGCAlloc(plgc_t* gc, size_t size){
	void* tempPtr;

	if((tempPtr = malloc(size)) == NULL){
		return NULL;
	}

	plGCManage(gc, PLGC_ADDPTR, tempPtr, size);
}

void* plGCCalloc(plgc_t* gc, size_t amount, size_t size){
	void* tempPtr;

	if((tempPtr = calloc(amount, size)) == NULL){
		return NULL;
	}

	plGCManage(gc, PLGC_ADDPTR, tempPtr, size);
}

void* plGCRealloc(plgc_t* gc, void* pointer, size_t size){
	void* tempPtr = realloc(pointer, size);

	if((tempPtr = realloc(pointer, size)) == NULL){
		return NULL;
	}

	plGCManage(gc, PLGC_RMPTR, tempPtr, size);
	plGCManage(gc, PLGC_ADDPTR, tempPtr, size);
}
