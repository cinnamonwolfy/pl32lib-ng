#include <pl32-calc.h>

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
		case '^': ;
			double tempNum = numbers[0];
			for(int i = 0; i < numbers[1]; i++){
				numbers[0] *= tempNum;
			}
			return numbers[0];
	}
}

// internal_calculate wrapper with order of operations algorithm
double* calculate(calcstruct_t workStruct){
	double* result = safe_malloc(sizeof(double));

	if((workStruct.size[1] + 1 != workStruct.size[0]) || (workStruct.size[0] <= 2 || workStruct.size[1] <= 1)){
		return NULL;
	}

	for(int i = 0; i < workStruct.size[1]; i++){
		if(workStruct.ops[i] == '*' || workStruct.ops[i] == '/'){
			if(i != 0 && (workStruct.ops[i - 1] == '*' || workStruct.ops[i - 1] == '/')){
				double tempNums[2] = {*result, workStruct.numbers[i + 1]};
				*result = internal_calculate(tempNums, workStruct.ops[i]);
				workStruct.numbers[i + 1] = 0;
			}else if(workStruct.size[1] == 1 || i == 0){
				double tempNums[2] = {workStruct.numbers[i], workStruct.numbers[i + 1]};
				*result = internal_calculate(tempNums, workStruct.ops[i]);
			}else{
				double tempNums[2] = {workStruct.numbers[i], workStruct.numbers[i + 1]};
				if(workStruct.ops[i - 1] == '-'){
					*result -= internal_calculate(tempNums, workStruct.ops[i]);
				}else if(workStruct.ops[i - 1] == '+'){
					*result += internal_calculate(tempNums, workStruct.ops[i]);
				}
				workStruct.numbers[i] = 0;
				workStruct.numbers[i + 1] = 0;
			}
		}
	}

	for(int i = 0; i < workStruct.size[1]; i++){
		if(workStruct.ops[i] == '+' || workStruct.ops[i] == '-'){
			if(*result == 0 || i == 0){
				double tempNums[2] = {workStruct.numbers[i], workStruct.numbers[i + 1]};
				*result += internal_calculate(tempNums, workStruct.ops[i]);
			}else{
				double tempNums[2] = {*result, workStruct.numbers[i + 1]};
				*result = internal_calculate(tempNums, workStruct.ops[i]);
			}
		}
	}
}

// parses a string containing a mathematical expression
double* calculateString(char* workString){
	parsedstr_t parsedWorkStr = parseString(workString, " ");
	safe_free(parsedWorkStr.array);
}

