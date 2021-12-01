// pl32-memory.c: Memory management and string parser module
#include <pl32-memory.h>

/* // Struct to track pointers and their sizes
typedef struct plptrtrack{
	void* pointer;
	size_t size;
} plptrtrack_t;

plptrtrack_t* pointerStore = NULL; // Contains the tracker structs
size_t ptrStoreSize = 0; // Size of pointerStore

size_t allocMaxMemory = 128 * 1024 * 1024; // Maximum amount of bytes it will allocate (Default = 128MB)
size_t usedMemory = 0; // Amount of currently malloc'd memory in bytes

// Finds a pointer within pointerStore. Returns -1 if it doesn't find the pointer
int findPtr(void* ptr){
	int i = 0;
	bool found;

	while(!found && i < ptrStoreSize){
		if(pointerStore[i].pointer == ptr){
			found = true;
		}else{
			i++;
		}
	}

	if(!found){
		return -1;
	}else{
		return i;
	}

}

// Adss a struct containing the pointer and the size of the pointer to pointerStore. Terminates the program on realloc() failure
void addPtrEntry(void* ptr, size_t size){
	if(pointerStore == NULL){
		pointerStore = malloc(2 * sizeof(plptrtrack_t));
		ptrStoreSize = 1;
	}

	plptrtrack_t tempTracker;

	tempTracker.pointer = ptr;
	tempTracker.size = size;

	plptrtrack_t* tempPtr = realloc(pointerStore, (ptrStoreSize + 1) * sizeof(plptrtrack_t));

	if(!tempPtr){
		exit(1);
	}else{
		pointerStore = tempPtr;
		ptrStoreSize++;

		pointerStore[ptrStoreSize - 1] = tempTracker;
		usedMemory += size;
	}

}

// Finds and removes a struct from pointerStore containing the pointer given. Terminates the program on realloc() failure
void rmPtrEntry(void* ptr){
	if(pointerStore == NULL){
		pointerStore = malloc(2 * sizeof(plptrtrack_t));
		ptrStoreSize = 1;
		return;
	}

	if(!ptr){
		return;
	}

	int i = findPtr(ptr);

	if(i != -1){
		usedMemory -= pointerStore[i].size;
		pointerStore[i].pointer = pointerStore[ptrStoreSize - 1].pointer;
		pointerStore[i].size = pointerStore[ptrStoreSize - 1].size;

		plptrtrack_t* tempPtr = realloc(pointerStore, (ptrStoreSize * sizeof(plptrtrack_t)) - sizeof(plptrtrack_t));

		if(!tempPtr){
			exit(1);
		}else{
			pointerStore = tempPtr;
			ptrStoreSize--;

		}
	}
}*/

// Memory buffer struct
struct plmembuf {
	void* pointer;
	size_t size;
};

// Pointer struct
struct plpointer {
	const void* pointer;
	size_t sizeOfItem;
};

// Array struct
struct plarray {
	plmembuf_t membuf;
	size_t sizeOfItem;
}

struct plgc {
	plarray_t pointerStore;
	size_t usedMemory;
	size_t maxMemory;
}

plgc_t garbageCollector;

int plInternalAddToPtrStore(void* retPtr, size_t size){
	
}

// Change memory allocation limit
void plChangeAllocLimit(size_t bytes){
	allocMaxMemory = bytes;
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


// plSafeMalloc but casted to int pointer
int* plIntSafeMalloc(size_t amount){
	return (int*)plSafemalloc(amount * sizeof(int));
}

// plSafeRealloc but casted to int pointer
int* plIntSafeRealloc(int* pointer, size_t amount){
	return (int*)plSaferealloc(pointer, amount * sizeof(int));
}

// plSafeMalloc but casted to char pointer
char* plCharSafeMalloc(size_t amount){
	return (char*)plSafemalloc(amount * sizeof(char));
}

// plSafeRealloc but casted to char pointer
char* plCharSafeRealloc(char* pointer, size_t amount){
	return (char*)plSaferealloc(pointer, amount * sizeof(char));
}
