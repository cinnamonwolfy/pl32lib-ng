// pl32lib wrapper for FILE*
struct plfile {
	FILE* fileptr; // File pointer for actual files
	char* strbuf; // String pointer for stringstream
	char* mode; // File open mode
	size_t seekbyte; // Byte offset from the beginning of buffer
	size_t bufsize; // Buffer size
};

plfile_t* plFOpen(char* filename, char* mode){
	plfile_t* returnStruct = NULL;

	if(mode){
		returnStruct = plGCMalloc(sizeof(plfile_t));
		if(!filename){
			returnStruct->fileptr = NULL;
		}else{
			returnStruct->fileptr = fopen(filename, mode);
		}

		returnStruct->strbuf = NULL;
		returnStruct->mode = plGCMalloc((strlen(mode)+1) * sizeof(char));
		strcpy(returnStruct->mode, mode);
		returnStruct->bufsize = 0;
	}

	return returnStruct;
}

int plFClose(plfile_t* ptr){
	if(!fileptr){
		plGCFree(ptr->strbuf);
	}else{
		fclose(ptr->fileptr);
	}

	plGCFree(ptr->mode);
	plGCFree(ptr);
}

size_t plFRead(const void* ptr, size_t size, size_t nmemb, plfile_t* stream){
	if(!stream->fileptr){
		int elementAmnt = 0;
		while(size * elementAmnt > stream->bufsize - stream->seekbyte){
			elementAmnt++;
		}
		elementAmnt--;

		if(!elementAmnt){
			return 0;
		}

		memcpy(ptr, stream->strbuf + seekbyte, size * elementAmnt);
		stream->seekbyte += size * elementAmnt;
	}else{
		return fread(ptr, size, nmemb, stream->fileptr);
	}
}

size_t plFWrite(const void* ptr, size_t size, size_t nmemb, plfile_t* stream){
	if(!stream->fileptr){
		if(size * nmemb > stream->bufsize - stream->seekbyte){
			void* tempPtr = realloc(stream->strbuf, stream->bufsize + size * nmemb);
			if(!tempPtr){
				return 0;
			}

			stream->strbuf = tempPtr;
		}

		memcpy(ptr, stream->strbuf + seekbyte, size * nmemb);
		stream->seekbyte += size * elementAmnt;
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
