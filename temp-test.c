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

	plstring_t matchStr = plUStrFromCStr("wo", NULL);
	plchar_t plChr = {
		.bytes = { 'w', '\0', '\0', '\0'  }
	};

	int64_t retIndex = plUStrchr(&convertedStr, plChr, 0);
	if(retIndex == -1)
		plPanic("main: Offset is negative!", false, true);

	printf("Returned Offset: %ld\n", retIndex);
	printf("Current Value: %c\n", *((char*)convertedStr.data.array + retIndex));

	retIndex = plUStrstr(&convertedStr, &matchStr, 5);
	if(retIndex == -1)
		plPanic("main: Offset is negative!", false, true);

	printf("Returned Offset: %ld\n", retIndex);
	fputs("Current Value: ", stdout);
	fwrite(convertedStr.data.array + retIndex, 1, 8, stdout);
	fputs("\n", stdout);

	plstring_t tokenizedStr = plUStrtok(&convertedStr)

	plMTStop(mainMT);
	return 0;
}
