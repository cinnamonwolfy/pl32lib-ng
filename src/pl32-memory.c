#include <pl32-memory.h>

typedef struct pointerTracker{
	void* pointer;
	size_t size;
} ptrtrack_t;

ptrtrack_t* pointerStore = NULL;
size_t ptrStoreSize = 0;

size_t allocMaxMemory = 128 * 1024 * 1024;
size_t usedMemory = 0;

int findPtr(void* pointer){
	int i = 0;

	while(i < ptrStoreSize){
		if((pointer == NULL && pointerStore[i].pointer == NULL) || (pointerStore[i] != NULL && pointerStore[i].pointer == pointer)){
			return i;
		}

		i++;
	}

	return -1;
}

void ptrHandler(void* pointer, size_t size, unsigned int action){
	if(pointerStore == NULL){
		pointerStore = (ptrtrack_t*)malloc(1 * sizeof(ptrtrack_t));
	}

	switch(action){
		case 0:
			int index = findPtr(NULL);

			if(index == -1){
				pointerStore = (ptrtrack_t*)realloc(pointerStore, (ptrStoreSize + 1) * sizeof(ptrtrack_t));
				ptrStoreSize++;
				i = ptrStoreSize - 1;
			}

			ptrtrack_t tempTracker;

			tempTracker.pointer = pointer;
			tempTracker.size = size;

			pointerStore[index] = tempTracker;
			break;
		case 1:
			int index = findPtr(pointer);

			if(index == -1){
				return;
			}

			usedMemory = usedMemory - pointerStore[index].size;
			pointerStore[index].pointer = NULL;
			pointerStore[index].size = 0;
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
	ptrHandler(retPtr, size, 0);

	return retPtr;
}

void* safe_calloc(size_t amount, size_t size){
	if((usedMemory + size) > allocMaxMemory){
		return NULL;
	}

	void* retPtr = calloc(amount, size);
	ptrHandler(retPtr, size, 0);

	return retPtr;
}

void* safe_realloc(void* pointer, size_t size){
	int pIndex = findPtr(pointer);
	int newAllocSize = usedMemory;

	if(pIndex == -1){
		pointer = malloc(1);
		newAllocSize = newAllocSize + size;
	}else{
		newAllocSize = newAllocSize + ((int)size - ptrStore[pIndex].size);
	}

	if(NewAllocSize > allocMaxMemory){
		return NULL;
	}


	void* retPtr = realloc(pointer, size);
	ptrHandler(pointer, NULL, 1);
	ptrHandler(retPtr, size, 0);

	return retPtr;
}

void safe_free(void* pointer){
	ptrHandler(pointer, NULL, 1);
	free(pointer);
}

void safe_free_all(){
	for(int i = 0; i < ptrStoreSize; i++){
		if(pointerStore[i] != NULL){
			free(pointerStore[i].pointer);
			pointerStore[i] = NULL;
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

parsedstr_t parse(char* input, char delimiter){
	if(delimiter == NULL){
		delimiter = ' ';
	}

	char** workArr = safe_malloc(1 * sizeof(char*));
	int size = 1;
	char* workPtr = strtok(input, delimeter);
	workArr[0] = workPtr;

	while((workPtr = strtok(NULL, delimeter)) != NULL){
		size++;
		workArr = safe_realloc(size * sizeof(char*));
		workArr[size - 1] = workPtr;
	}

	parsedstr_t returnStruct;
	returnStruct.array = workArr;
	returnStruct.size = size;

	return returnStruct;
}
