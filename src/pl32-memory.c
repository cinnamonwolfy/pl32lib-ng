/********************************************\
* pl32lib, v0.01                             *
* (c)2021 pocketlinux32, Under Lesser GPLv3  *
* Memory Management/Garbage Collector module *
\********************************************/
#include <pl32-memory.h>

#define PLGC_INIT 0
#define PLGC_REQMEM 1
#define PLGC_REQMOREMEM 2
#define PLGC_FREEMEM 3

bool isInitialized = false;

// Memory Buffer
struct plmembuf {
	void* pointer;
	size_t size;
};

// Immutable Pointer
struct plpointer {
	union {
		const void* pointer;
		const long long unsigned int ptrAsInt;
	};
	size_t sizeOfItem;
};

// Variable Array
struct plarray {
	plmembuf_t membuf;
	size_t sizeOfItem;
};

// Garbage Collector
struct plgc {
	plmembuf_t memoryBuffer;
	plpointer_t* usedPointers;
	plpointer_t* freePointers;
	size_t usedPointersAmnt;
	size_t freePointersAmnt;
	size_t usedMemory;
	size_t maxMemory;
};

plgc_t mainGarbageCollector;

// Find a value in a memory buffer
void* plMemFindInMembuf(plmembuf_t membuf, plpointer_t pointer, int mode){
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

	return 0;
}

int plMemRequestMemory(plmembuf_t* membuf, size_t size, bool realloc_ptr){
	void tempPtr;

	if(realloc_ptr && !(tempPtr = realloc(membuf->pointer, size))){
		return 1;
	}else if(!(tempPtr = malloc(size))){
		return 2;
	}

	membuf->pointer = tempPtr;
	membuf->size = size;

	return 0;
}

int plMemRequestMoreMemory(plmembuf_t* membuf, size_t size){
	if(plMemRequestMemory(membuf, membuf->size + size, true)){
		return 1;
	}

	return 0;
}

plmembuf_t* plGCRequestMemory(size_t size){
	plmembuf_t retbuf;

	if(!isInitialized)
		return NULL;

	if(mainGarbageCollector.usedMemory + size > mainGarbageCollector.memomyBuffer.size){
		if(){
			plMemRequestMoreMemory(mainGarbageCollector.membuf&, size);
		}
	}
}

int plGCManage(int mode, plmembuf_t* membuf, size_t size){
	switch(mode){
		case PLGC_INIT:
			break;
		case PLGC_REQMEM:
			break;
		case PLGC_REQMOREMEM:
			break;
		case PLGC_FREEMEM:
			break;
	}
}

