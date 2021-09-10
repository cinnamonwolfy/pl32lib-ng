// pl32-memory.c: Memory management and string parser module
#include <pl32-memory.h>

// Struct to track pointers and their sizes
typedef struct ptrtrack{
	void* pointer;
	size_t size;
} ptrtrack_t;

ptrtrack_t* pointerStore = NULL; // Contains the tracker structs
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
		pointerStore = malloc(2 * sizeof(ptrtrack_t));
		ptrStoreSize = 1;
	}

	ptrtrack_t tempTracker;

	tempTracker.pointer = ptr;
	tempTracker.size = size;

	ptrtrack_t* tempPtr = realloc(pointerStore, (ptrStoreSize + 1) * sizeof(ptrtrack_t));

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
		pointerStore = malloc(2 * sizeof(ptrtrack_t));
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

		ptrtrack_t* tempPtr = realloc(pointerStore, (ptrStoreSize * sizeof(ptrtrack_t)) - sizeof(ptrtrack_t));

		if(!tempPtr){
			exit(1);
		}else{
			pointerStore = tempPtr;
			ptrStoreSize--;

		}
	}
}

// Change memory allocation limit
void changeAllocLimit(size_t bytes){
	allocMaxMemory = bytes;
}

// Get current amount of used memory
size_t getAllocSize(){
	return usedMemory;
}

// Malloc wrapper that adds a struct to pointerStore
void* safe_malloc(size_t size){
	if((usedMemory + size) > allocMaxMemory){
		return NULL;
	}

	void* retPtr = malloc(size);
	addPtrEntry(retPtr, size);

	return retPtr;
}

// Calloc wrapper that adds a struct to pointerStore
void* safe_calloc(size_t amount, size_t size){
	if((usedMemory + size) > allocMaxMemory){
		return NULL;
	}

	void* retPtr = calloc(amount, size);
	addPtrEntry(retPtr, size);

	return retPtr;
}

// Realloc wrapper that removes and add a struct to pointerStore
void* safe_realloc(void* pointer, size_t size){
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

// Free wrapper that removes a struct from pointerStore
void safe_free(void* pointer){
	rmPtrEntry(pointer);
	free(pointer);
}

// Frees all pointers listed in pointerStore
void safe_free_all(){
	for(int i = 0; i < ptrStoreSize; i++){
		if(pointerStore[i].pointer != NULL){
			free(pointerStore[i].pointer);
			pointerStore[i].pointer = NULL;
			pointerStore[i].size = 0;
		}
	}

	free(pointerStore);
	pointerStore = malloc(2 * sizeof(ptrtrack_t));
}

// safe_malloc but casted to int pointer
int* int_smalloc(size_t amount){
	return (int*)safe_malloc(amount * sizeof(int));
}

// safe_realloc but casted to int pointer
int* int_srealloc(int* pointer, size_t amount){
	return (int*)safe_realloc(pointer, amount * sizeof(int));
}

// safe_malloc but casted to char pointer
char* char_smalloc(size_t amount){
	return (char*)safe_malloc(amount * sizeof(char));
}

// safe_realloc but casted to char pointer
char* char_srealloc(char* pointer, size_t amount){
	return (char*)safe_realloc(pointer, amount * sizeof(char));
}
