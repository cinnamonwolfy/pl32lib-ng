#include <pl32-file.h>

int main(int argc, const char* argv[]){
	plgc_t* mainGC = plGCInit(8 * 1024 * 1024)
	char* stringBuffer = plGCAlloc(4096 * sizeof(char));

	printf("Opening an existing file...");
	plfile_t* realFile = plFOpen("include/pl32-file.h", "r", gc);
	plfile_t* memFile = plFOpen(NULL, "w+", gc);

	if(!realFile){
		printf("Error opening file. Exiting...\n");
	}

	printf("Done\n");
	plFGets(stringBuffer, 4095, realFile);
	stringBuffer[4095] = '\0';
	printf("Contents of include/pl32-file.h:\n%s", stringBuffer);

	printf("Reading and writing to file-in-memory")
	plFPuts()

	return 0;
}
