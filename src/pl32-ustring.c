/**********************************************\
 pl32lib-ng, v1.06
 (c) 2022-2023 pocketlinux32, Under MPL v2.0
 pl32-ustring.c: UTF-8 String ops module header
\**********************************************/
#include <pl32-ustring.h>

plstring_t plUStrFromCStr(string_t cStr, plmt_t* mt){
	if(cStr == NULL)
		plPanic("plStrFromCStr: NULL pointers given!", false, true);

	size_t cStrSize = strlen(cStr);
	memptr_t tempPtr = cStr;
	bool memAlloc = false;
	if(mt != NULL){
		tempPtr = plMTAlloc(mt, cStrSize);
		memAlloc = true;
		memcpy(tempPtr, cStr, cStrSize);
	}

	plstring_t retStruct = {
		.data = {
			.array = tempPtr,
			.size = cStrSize,
			.isMemAlloc = memAlloc,
			.mt = mt
		},
		.isplChar = false
	};

	return retStruct;
}

void plUStrCompress(plstring_t* plCharStr, plmt_t* mt){
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

memptr_t plMemMatch(plarray_t* memBlock1, plarray_t* memBlock2){
	if(memBlock1 == NULL || memBlock1->array == NULL || memBlock2 == NULL || memBlock2->array == NULL)
		plPanic("plMemMatch: Given memory block is NULL", false, true);

	uint8_t* mainPtr = memBlock1->array;
	uint8_t* searchPtr = memBlock2->array;
	for(int i = 0; i < memBlock1->size - memBlock2->size; i++){
		if(*(mainPtr + i) == *(searchPtr)){
			bool isThere = true;
			for(int j = 1; j < memBlock2->size; j++){
				if(*(mainPtr + i + j) != *(searchPtr)){
					isThere = false;
					j = memBlock2->size;
				}
			}

			if(isThere)
				return mainPtr + i;
		}
	}

	return NULL;
}

ssize_t plUStrchr(plstring_t* string, plchar_t chr){
	if(string == NULL || string->data.array == NULL)
		plPanic("plUStrchr: Given string is NULL!", false, true);
	if(string->isplChar)
		plPanic("plUStrchr: Given string is a plChar array", false, true);

	plarray_t tempStruct = {
		.array = chr.bytes,
		.size = 5,
		.isMemAlloc = false,
		.mt = NULL
	};

	memptr_t tempPtr = plMemMatch(string->data.array, &tempStruct);
	ssize_t retVar = -1;

	if(tempPtr != NULL)
		retVar = tempPtr - string->data.array;

	return retVar;
}

ssize_t plUStrstr(plstring_t* string1, plstring_t* string2){
	if(string1 == NULL || string1->data.array == NULL || string2 == NULL || string2->data.array == NULL)
		plPanic("plUStrstr: Given string is NULL!", false, true);
	if(string1->isplChar || string2->isplChar)
		plPanic("plUStrstr: Given string is a plChar array", false, true);

	memptr_t tempPtr = plMemMatch(string1->data.array, string2->data.array);
	ssize_t retVar = -1;

	if(tempPtr != NULL)
		retVar = tempPtr - string1->data.array;

	return retVar;
}

plstring_t plUStrtok(plstring_t* string, plstring_t* delimeter, plstring_t* leftoverStr, plmt_t* mt){
	plstring_t nano;
	return nano;
}

plstring_t plUStrdup(plstring_t* string, bool compress, plmt_t* mt){
	plstring_t nano;
	return nano;
}
