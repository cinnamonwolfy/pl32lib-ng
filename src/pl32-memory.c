/********************************************\
* pl32lib, v0.01                             *
* (c)2021 pocketlinux32, Under Lesser GPLv3  *
* Memory Management/Garbage Collector module *
\********************************************/
#include <pl32-memory.h>

// Memory buffer struct
struct plmembuf {
	void* pointer;
	size_t size;
};

// Pointer struct
struct plpointer {
	union {
		const void* pointer;
		const long long unsigned int ptrAsInt;
	};
	size_t sizeOfItem;
};

// Array struct
struct plarray {
	plmembuf_t membuf;
	size_t sizeOfItem;
};

// Garbage Collector
struct plgc {
	plarray_t pointerStore;
	size_t usedMemory;
	size_t maxMemory;
};

plgc_t* mainGarbageCollector = NULL;

// Find a value in a memory buffer
long long unsigned int plFindInMembuf(plmembuf_t membuf, plpointer_t pointer, int mode){
	int i = 0;
	bool done = false;

	switch(mode){
		case 0:
			while(i + pointer.sizeOfItem < membuf.size && !done){
				if(!memcmp(membuf.pointer + i, pointer.pointer, pointer.sizeOfItem)){
					return membuf.pointer + i;
				}
				i++;
			}
			break;
		case 1:
			while(i * pointer.sizeOfItem < membuf.size && !done){
				if(!memcmp(membuf.pointer + (i * pointer.sizeOfItem), pointer.pointer, pointer.sizeOfItem)){
					return membuf.pointer + (i * pointer.sizeOfItem);
				}
				i++;
			}
			break;
	}
}

int plManageGC(plgc_t* gc, int mode, ){
	
}

/*
// Change memory allocation limit
void plChangeAllocLimit(size_t bytes){
	
}

// Get current amount of used memory
size_t plGetAllocSize(){
	return usedMemory;
}

// Malloc wrapper that keeps track of pointers
void* plSafeMalloc(size_t size){
	if((usedMemory + size) > allocMaxMemory){
		return NULL;
	}

	void* retPtr = malloc(size);
	addPtrEntry(retPtr, size);

	return retPtr;
}

// Calloc wrapper that keeps track of pointers
void* plSafeCalloc(size_t amount, size_t size){
	if((usedMemory + size) > allocMaxMemory){
		return NULL;
	}

	void* retPtr = calloc(amount, size);
	addPtrEntry(retPtr, size);

	return retPtr;
}

// Realloc wrapper that keeps track of pointers
void* plSafeRealloc(void* pointer, size_t size){
	int pIndex = findPtr(pointer);
	int newAllocSize = usedMemory;

	if(pIndex == -1){
		pointer = malloc(2);
		newAllocSize = newAllocSize + size;
	}else{
		newAllocSize = newAllocSize + (size - pointerStore[pIndex].size);
	}

	if(newAllocSize > allocMaxMemory){
		return NULL;
	}


	void* retPtr = realloc(pointer, size);
	rmPtrEntry(pointer);
	addPtrEntry(retPtr, size);

	return retPtr;
}

// Free wrapper that keeps track of pointers
void plSafeFree(void* pointer){
	rmPtrEntry(pointer);
	free(pointer);
}

// Frees all remembered pointers
void plSafeFreeAll(){
	for(int i = 0; i < ptrStoreSize; i++){
		if(pointerStore[i].pointer != NULL){
			free(pointerStore[i].pointer);
			pointerStore[i].pointer = NULL;
			pointerStore[i].size = 0;
		}
	}

	free(pointerStore);
	pointerStore = malloc(2 * sizeof(plptrtrack_t));
}

int plAddToArray(plarray_t* array, plpointer_t pointer){
	if(pointer.sizeOfItem != array->sizeOfItem){
		return 1;
	}

	void* tempbuf = plSafeRealloc
}

int plAddPointerToArray(plarray_t* array, plpointer_t pointer){
	
}

int plRemovePointerFromArray(plarray_t* array, void* pointer){
	
}

*/
