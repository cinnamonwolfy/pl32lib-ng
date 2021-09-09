// pl32-parse.c: Parsing module
#include <pl32-string.h>

// A simple internal calculator
double internal_calculate(double* numbers, char op){
	switch(op){
		case '+':
			return numbers[0] + numbers[1];
		case '-':
			return numbers[0] - numbers[1];
		case '*':
			return numbers[0] * numbers[1];
		case '/':
			return numbers[0] / numbers[1];
		case '^':
			double tempNum = numbers[0];
			for(int i = 0; i < numbers[1]; i++){
				numbers[0] *= tempNum;
			}
			return numbers[0];
	}
}

// Parses a string into an array
parsedstr_t strparse(char* input, char* delimiter){
	if(delimiter == NULL){
		delimiter = malloc(1);
		*delimiter = ' ';
	}

	char** workArr = safe_malloc(2 * sizeof(char*));
	int size = 1;
	char* workPtr = strtok(input, delimiter);
	workArr[0] = workPtr;

	while((workPtr = strtok(NULL, delimiter)) != NULL){
		size++;
		void* tempArr = safe_realloc(workArr, size * sizeof(char*));
		if(!tempArr){
			parsedstr_t errStruct;
			errStruct.array = NULL;
			errStruct.size = 0;
			return errStruct;
		}
		workArr = tempArr;
		workArr[size - 1] = workPtr;
	}

	parsedstr_t returnStruct;
	returnStruct.array = workArr;
	returnStruct.size = size;

	free(delimiter);

	return returnStruct;
}

// internal_calculate wrapper with order of operations algorithm
double calculate(calcstruct_t workStruct){
	double result;

	if((workStruct.size[1] + 1 != workStruct.size[0]) || (workStruct.size[0] <= 2 || workStruct.size[1] <= 1)){
		return NULL;
	}

	for(int i = 0; i < workStruct.size[1]; i++){
		if(ops[i] == '*' || ops[i] == '/'){
			if(i != 0 && (ops[i - 1] == '*' || ops[i - 1] == '/')){
				double tempNums[2] = {result, workStruct.numbers[i + 1]};
				result = internal_calculate(tempNums, workStruct.ops[i]);
				numbers[i + 1] = 0;
			}else if(workStruct.size[1] == 1 || i == 0){
				double tempNums[2] = {numbers[i], numbers[i + 1]};
				result = internal_calculate(tempNums, workStruct.ops[i]);
			}else{
				double tempNums[2] = {workStruct.numbers[i], workStruct.numbers[i + 1]};
				if(workStruct.ops[i - 1] == '-'){
					result -= internal_calculate(tempNums, workStruct.ops[i]);
				}else if(workStruct.ops[i - 1] == '+'){
					result += internal_calculate(tempNums, workStruct.ops[i]);
				}
				numbers[i] = 0;
				numbers[i + 1] = 0;
			}
		}
	}

	for(int i = 0; i < workStruct.size[1]; i++){
		if(workStruct.ops[i] == '+' || workStruct.ops[i] == '-'){
			if(result == 0 || i == 0){
				double tempNums[2] = {workStruct.numbers[i], workStruct.numbers[i + 1]};
				result += internal_calculate(tempNums, workStruct.ops[i]);
			}else{
				double tempNums[2] = {result, workStruct.numbers[i + 1]};
				result = internal_calculate(tempNums, ops[i]);
			}
		}
	}
}

// parses a string containing a mathematical expression
double calculateString(char* workString){
	parsedstr_t parsedWorkStr = string_parse(workString, " ");
}
