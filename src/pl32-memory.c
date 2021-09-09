//pl32-memory.c: Memory management and string parser module
#include <pl32-memory.h>

typedef struct pointerTracker{
	void* pointer;
	size_t size;
} ptrtrack_t;

ptrtrack_t* pointerStore = NULL;
size_t ptrStoreSize = 0;

size_t allocMaxMemory = 128 * 1024 * 1024;
size_t usedMemory = 0;

int findPtr(void* ptr){
	int i = 0;
	bool found;

	while(!found && i < ptrStoreSize){
		if(pointerStore[i].pointer == ptr){
			found = true;
		}
	}

	if(!found){
		return -1;
	}else{
		return i;
	}

}

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
		exit(0);
	}else{
		pointerStore = tempPtr;
		ptrStoreSize++;

		pointerStore[ptrStoreSize - 1] = tempTracker;
	}

}

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
		pointerStore[i].pointer = NULL;
		pointerStore[i].size = 0;
	}
}

void changeAllocLimit(size_t bytes){
	allocMaxMemory = bytes;
}

size_t getAllocSize(){
	return usedMemory;
}

void* safe_malloc(size_t size){
	if((usedMemory + size) > allocMaxMemory){
		return NULL;
	}

	void* retPtr = malloc(size);
	addPtrEntry(retPtr, size);

	return retPtr;
}

void* safe_calloc(size_t amount, size_t size){
	if((usedMemory + size) > allocMaxMemory){
		return NULL;
	}

	void* retPtr = calloc(amount, size);
	addPtrEntry(retPtr, size);

	return retPtr;
}

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

void safe_free(void* pointer){
	rmPtrEntry(pointer);
	free(pointer);
}

void safe_free_all(){
	for(int i = 0; i < ptrStoreSize; i++){
		if(pointerStore[i].pointer != NULL){
			free(pointerStore[i].pointer);
			pointerStore[i].pointer = NULL;
			pointerStore[i].size = 0;
		}
	}
}

int* int_smalloc(size_t amount){
	return (int*)safe_malloc(amount * sizeof(int));
}

int* int_srealloc(int* pointer, size_t amount){
	return (int*)safe_realloc(pointer, amount * sizeof(int));
}

char* char_smalloc(size_t amount){
	return (char*)safe_malloc(amount * sizeof(char));
}

char* char_srealloc(char* pointer, size_t amount){
	return (char*)safe_realloc(pointer, amount * sizeof(char));
}

parsedstr_t parse(char* input, char* delimiter){
	if(delimiter == NULL){
		delimiter = malloc(1);
		*delimiter = ' ';
	}

	char** workArr = safe_malloc(2 * sizeof(char*));
	int size = 1;
	char* workPtr = strtok(input, delimiter);
	workArr[0] = workPtr;

	while((workPtr = strtok(NULL, delimiter)) != NULL){
		size++;
		void* tempArr = safe_realloc(workArr, size * sizeof(char*));
		if(!tempArr){
			parsedstr_t errStruct;
			errStruct.array = NULL;
			errStruct.size = 0;
			return errStruct;
		}
		workArr = tempArr;
		workArr[size - 1] = workPtr;
	}

	parsedstr_t returnStruct;
	returnStruct.array = workArr;
	returnStruct.size = size;

	free(delimiter);

	return returnStruct;
}
