/****************************************************\
 pl32lib-ng, v1.05
 (c) 2022-2023 pocketlinux32, Under MPL v2.0
 pl32-ustring.c: UTF-8 String ops module header
\****************************************************/
#include <pl32-ustring.h>

plstring_t plStrFromCStr(string_t cStr, plmt_t* mt){
	if(mt == NULL || cStr == NULL)
		plPanic("plStrFromCStr: NULL pointers given!", false, true);

	size_t cStrSize = strlen(cStr);
	plstring_t retStruct = {
		.data = {
			.array = plMTAllocE(mt, cStrSize);
			.size = cStrSize;
			.isMemAlloc = true;
			.mt = mt;
		}
		.isplChar = false;
	}

	memcpy(retStruct.data.array, cStr, retStruct.data.size);
	return plStrFromCStr;
}

void plStrCompress(plstring_t* plCharStr, plmt_t* mt){
	if(mt == NULL || cStr == NULL)
		plPanic("plStrCompress: NULL pointers given!", false, true);
	if(!plCharStr->isplChar)
		plPanic("plStrCompress: plCharStr is not a plChar string", false, true);

	
}

ssize_t plStrUChr(plstring_t* string, plchar_t chr){

}

ssize_t plStrUStr(plstring_t* string1, plstring_t* string2){

}

plstring_t plStrUDup(plstring_t* string, bool compress, plmt_t* mt){

}
