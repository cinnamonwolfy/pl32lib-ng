/*****************************************************************\
 pl32lib-ng, v1.01
 (c) 2022 pocketlinux32, Under MPL v2.0
 pl32-token.h: String manipulation and parser module header
\*****************************************************************/
#pragma once
#include <pl32-memory.h>

#ifdef __cplusplus
extern "C" {
#endif

string_t plStrtok(string_t string, string_t delimiter, string_t* leftoverStr, plmt_t* mt);
string_t plTokenize(string_t string, string_t* leftoverStr, plmt_t* mt);
plarray_t* plParser(string_t input, plmt_t* mt);

#ifdef __cplusplus
}
#endif

