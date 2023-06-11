/****************************************************\
 pl32lib-ng, v1.05
 (c) 2022-2023 pocketlinux32, Under MPL v2.0
 pl32-ustring.h: UTF-8 String ops module header
\****************************************************/
#pragma once
#include <pl32-memory.h>

plstring_t plStrFromCStr(string_t cStr, plmt_t* mt);
plstring_t plStrCompress(plstring_t* plCharStr, plmt_t* mt);
ssize_t plStrUChr(plstring_t* string, plchar_t chr);
uint32_t plStrUStr(plstring_t* string1, plstring_t string2);
plstring_t plStrUDup(plstring_t* string, bool compress, plmt_t* mt)
