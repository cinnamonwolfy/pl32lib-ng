// pl32-math.c: Math parsing and calculating module
#include <pl32-math.h>

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
	}
}

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

double calculateString(char* workString){
	char ** workChar = safe_malloc(1 * sizeof(char*));
	char* workPointer = strtok(
}
