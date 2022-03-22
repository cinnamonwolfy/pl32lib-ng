/********************************************\
* pl32lib, v0.04                             *
* (c)2022 pocketlinux32, Under Lesser GPLv3  *
* File Management Module                     *
* Warning: unfinished!                       *
\********************************************/
#include <pl32-file.h>

struct plfile {
	FILE* fileptr; // File pointer for actual files
	char* strbuf; // String pointer for stringstream
	char* mode; // File open mode
	size_t seekbyte; // Byte offset from the beginning of buffer
	size_t bufsize; // Buffer size
	plgc_t* gcptr; // pointer to GC (see pl32-memory.h)
};

plfile_t* plFOpen(char* filename, char* mode, plgc_t* gc){
	plfile_t* returnStruct = NULL;

	if(mode){
		returnStruct = plGCAlloc(gc, sizeof(plfile_t));
		if(!filename){
			returnStruct->fileptr = NULL;
			returnStruct->strbuf = plGCAlloc(gc, 4098);
			returnStruct->bufsize = 4098;
		}else{
			returnStruct->fileptr = fopen(filename, mode);
			returnStruct->bufsize = 0;
			returnStruct->strbuf = NULL;
		}

		returnStruct->gcptr = gc;
		returnStruct->seekbyte = 0;
		returnStruct->mode = plGCAlloc(gc, (strlen(mode)+1) * sizeof(char));
		strcpy(returnStruct->mode, mode);
	}

	return returnStruct;
}

plfile_t* plFToP(FILE* pointer, char* mode, plgc_t* gc){
	plfile_t* returnPointer = plFOpen(NULL, mode, gc);
	returnPointer->fileptr = pointer;
	returnPointer->bufsize = 0;
	plGCFree(gc, returnPointer->strbuf);
	return returnPointer;
}

int plFClose(plfile_t* ptr){
	if(!ptr->fileptr){
		plGCFree(ptr->gcptr, ptr->strbuf);
	}else{
		if(fclose(ptr->fileptr))
			return 1;
	}

	plGCFree(ptr->gcptr, ptr->mode);
	plGCFree(ptr->gcptr, ptr);
	return 0;
}

size_t plFRead(void* ptr, size_t size, size_t nmemb, plfile_t* stream){
	if(!stream->fileptr){
		int elementAmnt = 0;
		while(size * elementAmnt < stream->bufsize - stream->seekbyte){
			elementAmnt++;
		}
		elementAmnt--;

		if(!elementAmnt){
			return 0;
		}

		memcpy(ptr, stream->strbuf + stream->seekbyte, size * elementAmnt);
		stream->seekbyte += size * elementAmnt;
		return size * elementAmnt;
	}else{
		return fread(ptr, size, nmemb, stream->fileptr);
	}
}

size_t plFWrite(void* ptr, size_t size, size_t nmemb, plfile_t* stream){
	if(!stream->fileptr){
		if(size * nmemb < stream->bufsize - stream->seekbyte){
			void* tempPtr = plGCRealloc(stream->gcptr, stream->strbuf, stream->bufsize + size * nmemb);
			if(!tempPtr){
				return 0;
			}

			stream->strbuf = tempPtr;
		}

		memcpy(ptr, stream->strbuf + stream->seekbyte, size * nmemb);
		stream->seekbyte += size * nmemb;
		return size * nmemb;
	}else{
		return fwrite(ptr, size, nmemb, stream->fileptr);
	}
}

char plFPutC(char ch, plfile_t* stream){
	if(!plFWrite(&ch, sizeof(char), 1, stream)){
		return '\0';
	}else{
		return ch;
	}
}

char plFGetC(plfile_t* stream){
	char ch;
	if(!plFRead(&ch, sizeof(char), 1, stream)){
		return '\0';
	}else{
		return ch;
	}
}

int plFPuts(char* string, plfile_t* stream){
	if(!plFWrite(string, strlen(string)+1, 1, stream)){
		return 0;
	}else{
		return 1;
	}
}

int plFGets(char* string, int num, plfile_t* stream){
	if(!plFRead(string, num, 1, stream)){
		return 0;
	}else{
		return 1;
	}
}

int plFSeek(plfile_t* stream, long int offset, int whence){
	if(!stream->fileptr){
		switch(whence){
			case SEEK_SET:
				if(offset < stream->bufsize){
					stream->seekbyte = offset;
				}else{
					return 1;
				}
				break;
			case SEEK_CUR:
				if(stream->seekbyte + offset < stream->bufsize){
					stream->seekbyte += offset;
				}else{
					return 1;
				}
				break;
			case SEEK_END:
				if(stream->bufsize - offset >= 0){
					stream->seekbyte = stream->bufsize - offset;
				}else{
					return 1;
				}
				break;
			default:
				return 1;
		}

		return 0;
	}else{
		return fseek(stream->fileptr, offset, whence);
	}
}
