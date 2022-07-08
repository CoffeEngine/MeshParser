#ifndef _CAFFEINE_UTILS_H_
#define _CAFFEINE_UTILS_H_

#include "common.h"

bool is_number(char c);
bool char_is_oneof(char c, char* values, size_t values_len);
int read_int(char** str);
char* skip_char(char* str, char c);

#endif // !_CAFFEINE_UTILS_H_