#include <pl32-types.h>

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
	plarray_t pointerStore;
	size_t usedMemory;
	size_t maxMemory;
};

struct pltokenizedstr {
	char** array;
	size_t size;
}

// Function Pointer
struct plfunctionptr {
	int (*function)(int, char**);
	char* name;
}

