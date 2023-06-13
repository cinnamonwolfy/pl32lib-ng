/**********************************************\
 pl32lib-ng, v1.05
 (c) 2022-2023 pocketlinux32, Under MPL v2.0
 pl32-ustring.c: UTF-8 String ops module header
\**********************************************/
#include <pl32-ustring.h>

plstring_t plStrFromCStr(string_t cStr, plmt_t* mt){
	if(mt == NULL || cStr == NULL)
		plPanic("plStrFromCStr: NULL pointers given!", false, true);

	size_t cStrSize = strlen(cStr);
	plstring_t retStruct = {
		.data = {
			.array = plMTAllocE(mt, cStrSize),
			.size = cStrSize,
			.isMemAlloc = true,
			.mt = mt
		},
		.isplChar = false
	};

	memcpy(retStruct.data.array, cStr, retStruct.data.size);
	return retStruct;
}

void plStrCompress(plstring_t* plCharStr, plmt_t* mt){
	if(mt == NULL || plCharStr == NULL)
		plPanic("plStrCompress: NULL pointers given!", false, true);
	if(!plCharStr->isplChar)
		plPanic("plStrCompress: plCharStr is not a plChar string", false, true);

	plchar_t* plCharStrPtr = plCharStr->data.array;
	uint8_t* compressedStr = plMTAlloc(mt, plCharStr->data.size * 5);
	size_t offset = 0;
	for(int i = 0; i < plCharStr->data.size; i++){
		int endOfUtfChr = -1;
		uint8_t bytes[5];

		for(int j = 4; j > 0; j--){
			if(endOfUtfChr == -1 && plCharStrPtr[i].bytes[j] != 0)
				endOfUtfChr = j;
		}

		for(int j = 0; j < endOfUtfChr; j++)
			compressedStr[offset + j] = plCharStrPtr[i].bytes[j];

		offset += endOfUtfChr + 1;
	}

	void* resizedPtr = plMTRealloc(mt, compressedStr, offset);
	if(resizedPtr == NULL)
		plPanic("plStrCompress: Failed to reallocate memory", false, false);

	compressedStr = resizedPtr;
	if(plCharStr->data.isMemAlloc && plCharStr->data.mt != NULL)
		plMTFree(mt, plCharStr->data.array);

	plCharStr->data.array = compressedStr;
	plCharStr->data.isMemAlloc = true;
	plCharStr->data.mt = mt;
}

ssize_t plStrUChr(plstring_t* string, plchar_t chr){
	return 0;
}

ssize_t plStrUStr(plstring_t* string1, plstring_t* string2){
	return 0;
}

plstring_t plStrUDup(plstring_t* string, bool compress, plmt_t* mt){
	plstring_t nano;
	return nano;
}
