#include <pl32-memory.h>

struct pointerTracker{
	void* pointer;
	size_t size;
}

struct pointerTracker* pointerStore = NULL;
unsigned int ptrStoreSize = 0;

unsigned int allocMaxMemory = 128 * 1024 * 1024;
unsigned int usedMemory = 0;

int findPtr(void* pointer){
	int i = 0;

	while(i < ptrStoreSize){
		if((pointer == NULL && pointerStore[i] == NULL) || (pointerStore[i] != NULL && pointerStore[i].pointer == pointer)){
			return i;
		}

		i++;
	}

	return -1;
}

void ptrHandler(void* pointer, size_t size, unsigned int action){
	if(pointerStore == NULL){
		pointerStore = malloc(1 * sizeof(struct pointerTracker));
	}

	switch(action){
		case 0:
			int i = findPtr(NULL);

			if(index == -1){
				pointerStore = realloc(pointerStore, (ptrStoreSize + 1) * sizeof(struct pointerTracker));
				ptrStoreSize++;
			}

			struct pointerTracker tempTracker;

			tempTracker.pointer = pointer;
			tempTracker.size = size;

			pointerStore[i] = tempTracker;
			break;
		case 1:
			int index = findPtr(pointer);

			if(index == -1){
				return;
			}

			usedMemory = usedMemory - pointerStore[index].size;
			pointerStore[index] = NULL;
	}
}

void changeAllocLimit(unsigned int bytes){
	allocMaxMemory = bytes;
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
	int newAllocSize = usedMemeory;

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
	free();
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
