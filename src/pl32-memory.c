/*************************************************\
* pl32lib, v0.01                                  *
* (c)2021 pocketlinux32, Under Lesser GPLv3       *
* Memory Management/Semi-Garbage Collector module *
\*************************************************/
#include <pl32-memory.h>

// Internal type for representing internal pointer references
typedef struct plpointer {
	void* pointer;
	size_t size;
} plptr_t;

// Structure of the semi-garbage collector. This semi-garbage collector is thread-specific
struct plgc {
	plptr_t* usedPointers;
	plptr_t* freedPointers;
	size_t upAmnt;
	size_t fpAmnt;
	size_t usedMemory;
	size_t maxMemory;
	bool isInitialized;
};

// Controller for semi-garbage collector
int plGCManage(plgc_t* gc, int mode, void* ptr, size_t size){
	if(gc == NULL){
		return 1;
	}

	switch(mode){
		// Initializes the semi-garbage collector
		case PLGC_INIT: ;
			if(!gc->isInitialized){
				gc->usedPointers = malloc(2 * sizeof(plptr_t));
				gc->freedPointers = malloc(2 * sizeof(plptr_t));
				gc->upAmnt = 0;
				gc->fpAmnt = 0;
				gc->usedMemory = 0;
				gc->maxMemory = 128 * 1024 * 1024;
				gc->isInitialized = true;
			}
			break;
		// Cleans memory and deallocates the semi-garbage collector
		case PLGC_STOP: ;
			for(int i = 0; i < gc->upAmnt; i++){
				free(gc->usedPointers[i].pointer);
			}
			free(gc->usedPointers);
			free(gc->freedPointers);
			gc->usedPointers = NULL;
			gc->freedPointers = NULL;
			gc->isInitialized = false;
			break;
		//Searches pointer address within the usedPointers and freedPointers arrays
		case PLGC_SEARCHPTR: ;
			int i = 0;
			while(gc->usedPointers[i].pointer != ptr && i < gc->upAmnt)
				i++;

			if(gc->usedPointers[i].pointer == ptr)
				return i;

			while(gc->freedPointers[i].pointer != ptr && i < gc->fpAmnt)
				i++;

			if(gc->freedPointers[i].pointer == ptr)
				return gc->upAmnt + i;

			return -1;
		// Adds pointer reference to the tracking array
		case PLGC_ADDPTR: ;
			if(gc->upAmnt > 1){
				void* tempPtr = realloc(gc->usedPointers, (gc->upAmnt + 1) * sizeof(plptr_t));

				if(!tempPtr)
					return 1;

				gc->usedPointers = tempPtr;
			}

			gc->usedPointers[gc->upAmnt].pointer = ptr;
			gc->usedPointers[gc->upAmnt].size = size;
			gc->upAmnt++;
			gc->usedMemory += size;
			break;
		// Removes pointer reference from the tracking array
		case PLGC_RMPTR: ;
			int searchresult = plGCManage(gc, PLGC_SEARCHPTR, ptr, 0);
			if(searchresult > gc->upAmnt || searchresult == -1)
				return 1;

			if(gc->fpAmnt){
				void* tempPtr = realloc(gc->freedPointers, (gc->fpAmnt + 1) * sizeof(plptr_t));

				if(!tempPtr)
					return 1;
			}

			gc->freedPointers[gc->fpAmnt].pointer = gc->usedPointers[searchresult].pointer;
			gc->freedPointers[gc->fpAmnt].size = gc->usedPointers[searchresult].size;
			gc->fpAmnt++;

			gc->usedMemory -= gc->usedPointers[searchresult].size;

			if(gc->upAmnt > 1){
				gc->usedPointers[searchresult].pointer = gc->usedPointers[gc->upAmnt - 1].pointer;
				gc->usedPointers[searchresult].size = gc->usedPointers[gc->upAmnt - 1].size;
				void* tempPtr = realloc(gc->usedPointers, (gc->upAmnt - 1) * sizeof(plptr_t));

				if(tempPtr)
					gc->usedPointers = tempPtr;
			}else{
				free(gc->usedPointers);
				gc->usedPointers = malloc(2 * sizeof(plptr_t));
			}

			gc->upAmnt--;

			if(size)
				free(ptr);

			break;
	}
	return 0;
}

size_t plGCMemAmnt(plgc_t* gc, int action, size_t size){
	switch(action){
		case PLGC_GET_USEDMEM: ;
			return gc->usedMemory;
			break;
		case PLGC_SET_MAXMEM: ;
			gc->maxMemory = size;
			break;
	}
	return 0;
}

// malloc() wrapper that interfaces with the semi-garbage collector
void* plGCAlloc(plgc_t* gc, size_t size){
	void* tempPtr;

	if((tempPtr = malloc(size)) == NULL)
		return NULL;

	plGCManage(gc, PLGC_ADDPTR, tempPtr, size);
	return tempPtr;
}

// calloc() wrapper that interfaces with the semi-garbage collector
void* plGCCalloc(plgc_t* gc, size_t amount, size_t size){
	void* tempPtr;

	if((tempPtr = calloc(amount, size)) == NULL)
		return NULL;

	plGCManage(gc, PLGC_ADDPTR, tempPtr, size);
	return tempPtr;
}

// realloc() wrapper that interfaces with the semi-garbage collector
void* plGCRealloc(plgc_t* gc, void* pointer, size_t size){
	void* tempPtr = realloc(pointer, size);

	if((tempPtr = realloc(pointer, size)) == NULL)
		return NULL;

	if(tempPtr == pointer){
		plGCManage(gc, PLGC_MODPTR, pointer, size);
	}else{
		plGCManage(gc, PLGC_RMPTR, pointer, 0);
		plGCManage(gc, PLGC_ADDPTR, tempPtr, size);
	}

	return tempPtr;
}

// free() wrapper that interfaces with the semi-garbage collector
void plGCFree(plgc_t* gc, void* pointer){
	plGCManage(gc, PLGC_RMPTR, pointer, 1);
}
