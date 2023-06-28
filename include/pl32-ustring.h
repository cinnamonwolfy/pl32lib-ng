/**********************************************\
 pl32lib-ng, v1.06
 (c) 2022-2023 pocketlinux32, Under MPL v2.0
 pl32-ustring.h: UTF-8 String ops module header
\**********************************************/
#pragma once
#include <pl32-memory.h>

plstring_t plStrFromCStr(string_t cStr, plmt_t* mt);
void plStrCompress(plstring_t* plCharStr, plmt_t* mt);
int64_t plUStrchr(plstring_t* string, plchar_t chr);
int64_t plUStrstr(plstring_t* string1, plstring_t* string2);
plstring_t plUStrtok(plstring_t* string, plstring_t* delimeter, plstring_t* leftoverStr);
plstring_t plUStrdup(plstring_t* string, bool compress, plmt_t* mt);
