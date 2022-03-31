/*************************************************\
* pl32lib, v0.05                                  *
* (c)2022 pocketlinux32, Under Lesser GPLv2.1     *
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
	plptr_t* ptrList;
	size_t listAmnt;
	size_t allocListAmnt;
	size_t usedMemory;
	size_t maxMemory;
};

// Creates and initializes a semi-garbage collector
plgc_t* plGCInit(size_t maxMemoryInit){
	plgc_t* returnGC = malloc(sizeof(plgc_t));
	returnGC->ptrList = malloc(2 * sizeof(plptr_t));
	returnGC->listAmnt = 0;
	returnGC->allocListAmnt = 2;
	returnGC->usedMemory = 0;

	if(!maxMemoryInit){
		returnGC->maxMemory = 128 * 1024 * 1024;
	}else{
		returnGC->maxMemory = maxMemoryInit;
	}

	return returnGC;
}

// Frees all pointers currently in
void plGCStop(plgc_t* gc){
	for(int i = 0; i < gc->listAmnt; i++){
		free(gc->ptrList[i].pointer);
	}
	free(gc->ptrList);
	free(gc);
}

// Controller for semi-garbage collector
int plGCManage(plgc_t* gc, int mode, void* ptr, size_t size, void* ptr2){
	if(gc == NULL){
		return 1;
	}

	switch(mode){
		//Searches pointer address within the tracking array
		case PLGC_SEARCHPTR: ;
			int i = 0;
			while(gc->ptrList[i].pointer != ptr && i < gc->listAmnt)
				i++;

			if(gc->ptrList[i].pointer == ptr)
				return i;

			return -1;
		// Adds pointer reference to the tracking array
		case PLGC_ADDPTR: ;
			if(gc->listAmnt >= gc->allocListAmnt){
				void* tempPtr = realloc(gc->ptrList, (gc->listAmnt + 1) * sizeof(plptr_t));

				if(!tempPtr)
					return 1;

				gc->ptrList = tempPtr;
				gc->allocListAmnt++;
			}

			gc->ptrList[gc->listAmnt].pointer = ptr;
			gc->ptrList[gc->listAmnt].size = size;
			gc->listAmnt++;
			gc->usedMemory += size;
			break;
		// Removes pointer reference from the tracking array
		case PLGC_RMPTR: ;
			int rmPtrResult = plGCManage(gc, PLGC_SEARCHPTR, ptr, 0, NULL);
			if(rmPtrResult == -1)
				return 1;

			gc->usedMemory -= gc->ptrList[rmPtrResult].size;
			gc->ptrList[rmPtrResult].pointer = gc->ptrList[gc->listAmnt - 1].pointer;
			gc->ptrList[rmPtrResult].size = gc->ptrList[gc->listAmnt - 1].size;
			gc->ptrList[gc->listAmnt - 1].pointer = NULL;
			gc->ptrList[gc->listAmnt - 1].size = 0;
			gc->listAmnt--;

			free(ptr);

			break;
		// Special mode for just realloc()
		case PLGC_REALLOC: ;
			int reallocResult = plGCManage(gc, PLGC_SEARCHPTR, ptr, 0, NULL);
			if(reallocResult == -1)
				return 1;

			gc->ptrList[reallocResult].pointer = ptr2;
			gc->usedMemory += (size - gc->ptrList[reallocResult].size);
			gc->ptrList[reallocResult].size = size;
			break;
		default:
			return 1;
	}
	return 0;
}

size_t plGCMemAmnt(plgc_t* gc, int action, size_t size){
	switch(action){
		case PLGC_GET_USEDMEM: ;
			return gc->usedMemory;
		case PLGC_GET_MAXMEM: ;
			return gc->maxMemory;
		case PLGC_SET_MAXMEM: ;
			gc->maxMemory = size;
			break;
	}
	return 0;
}

// malloc() wrapper that interfaces with the semi-garbage collector
void* plGCAlloc(plgc_t* gc, size_t size){
	void* tempPtr;

	if(gc->usedMemory + size > gc->maxMemory || (tempPtr = malloc(size)) == NULL)
		return NULL;

	if(plGCManage(gc, PLGC_ADDPTR, tempPtr, size, NULL)){
		free(tempPtr);
		return NULL;
	}

	return tempPtr;
}

// calloc() wrapper that interfaces with the semi-garbage collector
void* plGCCalloc(plgc_t* gc, size_t amount, size_t size){
	void* tempPtr;

	if(gc->usedMemory + size > gc->maxMemory || (tempPtr = calloc(amount, size)) == NULL)
		return NULL;

	if(plGCManage(gc, PLGC_ADDPTR, tempPtr, size, NULL)){
		free(tempPtr);
		return NULL;
	}

	return tempPtr;
}

// realloc() wrapper that interfaces with the semi-garbage collector
void* plGCRealloc(plgc_t* gc, void* pointer, size_t size){
	void* tempPtr;

	if(gc->usedMemory + size > gc->maxMemory || (tempPtr = realloc(pointer, size)) == NULL)
		return NULL;

	if(plGCManage(gc, PLGC_REALLOC, pointer, size, tempPtr)){
		free(tempPtr);
		return NULL;
	}

	return tempPtr;
}

// free() wrapper that interfaces with the semi-garbage collector
void plGCFree(plgc_t* gc, void* pointer){
	plGCManage(gc, PLGC_RMPTR, pointer, 0, NULL);
}
