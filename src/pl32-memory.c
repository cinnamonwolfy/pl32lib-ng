/********************************************\
* pl32lib, v0.01                             *
* (c)2021 pocketlinux32, Under Lesser GPLv3  *
* Memory Management/Garbage Collector module *
\********************************************/
#include <pl32-memory.h>

typedef struct plpointer {
	void* pointer;
	size_t size;
} plptr_t

// Garbage Collector Structure
struct plgc {
	
	size_t usedMemory;
	size_t maxMemory;
	bool isInitialized;
};

int plGCManage(plgc_t* gc, int mode, void* ptr){
	if(gc == NULL){
		return 1;
	}

	switch(mode){
		case PLGC_INIT:
			if(!gc->isInitialized){
				
				gc->usedMemory = 0;
				gc->maxMemory = 128 * 1024 * 1024;
				gc->isInitialized = true;
			}
			break;
		case PLGC_CLEAN:
			break;
		case PLGC_STOP:
			break;
	}
}


