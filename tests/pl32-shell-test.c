#include <pl32-shell.h>



int plShellTest(plarray_t* args, plgc_t* gc){
	char* holder;
	char* token;
	char* testStrings[5] = { "oneword", "two words", "\"multiple words enclosed by quotes\" not anymore lol", "\"quotes at the beginning\" some stuff in the middle \" and now quotes at the back\"", "\"just quotes lol\"" };

	printf("This is a test of the new pl32-shell tokenizer\n");

	printf("Test 1");
	testLoop(testStrings[0]);
}

int main(int argc, const char* argv[]){
	plShellInteractive(NULL);
}
