#include <pl32-file.h>

int main(int argc, const char* argv[]){
	plgc_t* mainGC = plGCInit(8 * 1024 * 1024);
	char stringBuffer[4096] = "";

	printf("Opening an existing file...");
	plfile_t* realFile = plFOpen("include/pl32-file.h", "r", mainGC);
	plfile_t* memFile = plFOpen(NULL, "w+", mainGC);

	if(!realFile){
		printf("Error opening file. Exiting...\n");
	}

	printf("Done\n");
	plFGets(stringBuffer, 4095, realFile);
	printf("Contents of include/pl32-file.h:\n%s\n", stringBuffer);

	printf("Reading and writing to file-in-memory...");
	plFPuts("test string getting sent to the yes", memFile);
	plFSeek(memFile, 0, SEEK_SET);
	plFGets(stringBuffer, 4095, memFile);
	printf("Done\n");
	printf("Contents of file-in-memory:\n%s\n", stringBuffer);

	return 0;
}
