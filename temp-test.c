#include <pl32-ustring.h>

int main(int argc, const char* argv[]){
	plmt_t* mainMT = plMTInit(1024 * 1024);
	plstring_t convertedStr = plUStrFromCStr("hewwo wowwd :3", mainMT);
	plstring_t plCharString = {
		.data = {
			.array = plMTAlloc(mainMT, 14 * sizeof(plchar_t)),
			.size = 14,
			.isMemAlloc = true,
			.mt = mainMT
		},
		.isplChar = true
	};

	((plchar_t*)plCharString.data.array)[0].bytes[0] = 'h';
	((plchar_t*)plCharString.data.array)[1].bytes[0] = 'e';
	((plchar_t*)plCharString.data.array)[2].bytes[0] = 'w';
	((plchar_t*)plCharString.data.array)[3].bytes[0] = 'w';
	((plchar_t*)plCharString.data.array)[4].bytes[0] = 'o';
	((plchar_t*)plCharString.data.array)[5].bytes[0] = ' ';
	((plchar_t*)plCharString.data.array)[6].bytes[0] = 'w';
	((plchar_t*)plCharString.data.array)[7].bytes[0] = 'o';
	((plchar_t*)plCharString.data.array)[8].bytes[0] = 'w';
	((plchar_t*)plCharString.data.array)[9].bytes[0] = 'w';
	((plchar_t*)plCharString.data.array)[10].bytes[0] = 'd';
	((plchar_t*)plCharString.data.array)[11].bytes[0] = ' ';
	((plchar_t*)plCharString.data.array)[12].bytes[0] = ':';
	((plchar_t*)plCharString.data.array)[13].bytes[0] = '3';
	plUStrCompress(&plCharString, mainMT);

	fwrite(convertedStr.data.array, 1, convertedStr.data.size, stdout);
	fputs("\n", stdout);
	fwrite(plCharString.data.array, 1, plCharString.data.size, stdout);
	fputs("\n", stdout);

	plarray_t matchStr = {
		.array = plMTAlloc(mainMT, 2),
		.size = 2,
		.isMemAlloc = true,
		.mt = mainMT
	};

	memcpy(matchStr.array, "wo", 2);
	memptr_t retPtr = plMemMatch(&convertedStr.data, &matchStr);
	if(retPtr == NULL)
		plPanic("main: retPtr is NULL!", false, true);

	printf("Original Pointer: %p\n", convertedStr.data.array);
	printf("Returned Pointer: %p\n", retPtr);
	fputs("Current Value: ", stdout);
	fwrite(retPtr, 1, 11, stdout);
	fputs("\n", stdout);

	plMTStop(mainMT);
	return 0;
}
