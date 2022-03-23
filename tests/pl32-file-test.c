#include <pl32-file.h>

int main(int argc, const char* argv[]){
	plgc_t* mainGC = plGCInit(8 * 1024 * 1024)

	printf("Opening an existing file")
	plfile_t* realFile = plFOpen("include/pl32-file.h", "r", gc);
	plfile_t* memFile = plFOpen(NULL, "w+", gc);

	if(!realFile){
		printf("Error opening file. Exiting...");
	}

	return 0;
}
