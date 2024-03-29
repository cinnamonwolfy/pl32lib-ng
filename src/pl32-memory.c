/*****************************************************\
 pl32lib-ng, v1.05
 (c) 2022 pocketlinux32, Under MPL v2.0
 pl32-memory.c: Safe memory management module
\*****************************************************/
#include <pl32-memory.h>

/* Internal enum for plMTManage() */
typedef enum plmtiaction {
	PLMT_STOP,
	PLMT_SEARCHPTR,
	PLMT_ADDPTR,
	PLMT_RMPTR,
	PLMT_REALLOC,
} plmtiaction_t;

/* Internal type for representing internal pointer references */
typedef struct plpointer {
	memptr_t pointer;
	size_t size;
} plptr_t;

/* Structure of the memory allocation tracker. This memory allocation tracker is thread-specific */
struct plmt {
	plptr_t* ptrList;
	size_t listAmnt;
	size_t allocListAmnt;
	size_t usedMemory;
	size_t maxMemory;
};

/* Prints an error and aborts the program. Within pl32-memory, it's used whenever malloc fails */
void plPanic(string_t msg, bool usePerror, bool developerBug){
	fputs("* Panic at function ", stderr);
	if(usePerror){
		perror(msg);
	}else{
		fputs(msg, stderr);
		fputs("\n", stderr);
	}

	if(developerBug)
		fputs("* If you're seeing this message, this is a bug. Please report this error to the project maintainers\n", stderr);

	abort();
}

/* Creates and initializes a memory allocation tracker */
plmt_t* plMTInit(size_t maxMemoryInit){
	plmt_t* returnMT = malloc(sizeof(plmt_t));
	returnMT->ptrList = malloc(2 * sizeof(plptr_t));
	returnMT->listAmnt = 0;
	returnMT->allocListAmnt = 2;
	returnMT->usedMemory = 0;

	if(returnMT == NULL || returnMT->ptrList == NULL)
		plPanic("plMTInit: Failed to allocate memory", false, false);

	if(!maxMemoryInit){
		returnMT->maxMemory = 128 * 1024 * 1024;
	}else{
		returnMT->maxMemory = maxMemoryInit;
	}

	return returnMT;
}

/* Frees all pointers currently in the memory allocation tracker and the tracker itself */
void plMTStop(plmt_t* mt){
	for(int i = 0; i < mt->listAmnt; i++){
		free(mt->ptrList[i].pointer);
	}
	free(mt->ptrList);
	free(mt);
}

/* An internal control function for the memory allocation tracker */
int plMTManage(plmt_t* mt, plmtiaction_t mode, memptr_t ptr, size_t size){
	if(mt == NULL){
		return 1;
	}

	switch(mode){
		/* Searches pointer address within the tracking array */
		case PLMT_SEARCHPTR: ;
			int i = 0;
			while(mt->ptrList[i].pointer != ptr && i < mt->listAmnt)
				i++;

			if(mt->ptrList[i].pointer == ptr)
				return i;

			return -1;
		/* Adds pointer reference to the tracking array */
		case PLMT_ADDPTR:
			if(mt->listAmnt >= mt->allocListAmnt){
				memptr_t tempPtr = realloc(mt->ptrList, (mt->listAmnt + 1) * sizeof(plptr_t));

				if(tempPtr == NULL)
					plPanic("plMTManage: Failed to resize array", false, false);

				mt->ptrList = tempPtr;
				mt->allocListAmnt++;
			}

			mt->ptrList[mt->listAmnt].pointer = ptr;
			mt->ptrList[mt->listAmnt].size = size;
			mt->listAmnt++;
			mt->usedMemory += size;
			break;
		/* Removes pointer reference from the tracking array */
		case PLMT_RMPTR:
			if(ptr == NULL)
				return 1;

			int rmPtrResult = plMTManage(mt, PLMT_SEARCHPTR, ptr, 0);
			if(rmPtrResult == -1)
				return 1;

			mt->usedMemory -= mt->ptrList[rmPtrResult].size;
			mt->ptrList[rmPtrResult].pointer = mt->ptrList[mt->listAmnt - 1].pointer;
			mt->ptrList[rmPtrResult].size = mt->ptrList[mt->listAmnt - 1].size;
			mt->ptrList[mt->listAmnt - 1].pointer = NULL;
			mt->ptrList[mt->listAmnt - 1].size = 0;
			mt->listAmnt--;

			free(ptr);

			break;
		/* Special mode for just realloc() */
		case PLMT_REALLOC: ;
			int reallocResult = plMTManage(mt, PLMT_SEARCHPTR, *((void**)ptr), 0);
			if(reallocResult == -1)
				return 1;

			void* tempPtr = realloc(*(void**)ptr, size);
			if(tempPtr == NULL)
				plPanic("plMTManage: Couldn't reallocate memory", false, false);

			mt->ptrList[reallocResult].pointer = tempPtr;
			mt->usedMemory += (int)(size - mt->ptrList[reallocResult].size);
			mt->ptrList[reallocResult].size = size;

			*((void**)ptr) = tempPtr;
			break;
		default:
			return 1;
	}
	return 0;
}

/* Get the current memory usage or the maximum memory usage limit, or set a new *\
\* maximum memory usage limit                                                   */
size_t plMTMemAmnt(plmt_t* mt, plmtaction_t action, size_t size){
	switch(action){
		case PLMT_GET_USEDMEM:
			return mt->usedMemory;
		case PLMT_GET_MAXMEM:
			return mt->maxMemory;
		case PLMT_SET_MAXMEM:
			mt->maxMemory = size;
			break;
	}
	return 0;
}

/* malloc() wrapper that interfaces with the memory allocation tracker */
memptr_t plMTAlloc(plmt_t* mt, size_t size){
	memptr_t tempPtr;

	if(mt == NULL || mt->usedMemory + size > mt->maxMemory || (tempPtr = malloc(size)) == NULL)
		return NULL;

	plMTManage(mt, PLMT_ADDPTR, tempPtr, size);
	return tempPtr;
}

/* plMTAlloc() wrapper that mimics BSD's emalloc behavior */
memptr_t plMTAllocE(plmt_t* mt, size_t size){
	memptr_t tempPtr = plMTAlloc(mt, size);

	if(tempPtr == NULL)
		plPanic("plMTAllocE: Failed to allocate memory", false, false);

	return tempPtr;
}

/* calloc() wrapper that interfaces with the memory allocation tracker */
memptr_t plMTCalloc(plmt_t* mt, size_t amount, size_t size){
	memptr_t tempPtr;

	if(mt == NULL|| mt->usedMemory + size > mt->maxMemory || (tempPtr = calloc(amount, size)) == NULL)
		return NULL;

	plMTManage(mt, PLMT_ADDPTR, tempPtr, size);
	return tempPtr;
}

/* realloc() wrapper that interfaces with the memory allocation tracker */
memptr_t plMTRealloc(plmt_t* mt, memptr_t pointer, size_t size){
	memptr_t tempPtr = pointer;

	if(mt == NULL || mt->usedMemory + size > mt->maxMemory)
		return NULL;

	if(plMTManage(mt, PLMT_REALLOC, &tempPtr, size))
		return NULL;

	return tempPtr;
}

/* free() wrapper that interfaces with the memory allocation tracker */
void plMTFree(plmt_t* mt, memptr_t pointer){
	plMTManage(mt, PLMT_RMPTR, pointer, 0);
}

/* Frees a plarray_t */
void plMTFreeArray(plarray_t* array, bool is2DArray){
	if(array == NULL || array->mt == NULL)
		return;

	if(is2DArray){
		for(int i = 0; i < array->size; i++)
			plMTFree(array->mt, ((memptr_t*)array->array)[i]);
	}
	plMTFree(array->mt, array->array);
}
