/*****************************************************************\
 pl32lib-ng, v0.95
 (c) 2022 pocketlinux32, Under MPL v2.0
 pl32-token.h: String manipulation and parser module header
\*****************************************************************/
#pragma once
#include <pl32-memory.h>

#ifdef __cplusplus
extern "C" {
#endif

char* plStrtok(char* string, char* delimiter, char** leftoverStr, plmt_t* mt);
char* plTokenizeStrtok(char* string, char** leftoverStr, plmt_t* mt);
char* plTokenize(char* string, char** leftoverStr, plmt_t* mt);
plarray_t* plParser(char* input, plmt_t* mt);

#ifdef __cplusplus
}
#endif

