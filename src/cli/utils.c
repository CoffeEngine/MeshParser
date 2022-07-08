#include "utils.h"

bool is_number(char c) {
	return (0 <= c - '0' && c - '0' <= 9) || c == '.' || c == '-' || c == '+' || c == 'e' || c == 'E';
}

bool char_is_oneof(char c, char* values, size_t values_len) {
	for (size_t i = 0; i < values_len; i++)
	{
		if (c == values[i]) return true;
	}
	return false;
}

int read_int(char** str) {
	char buff[32];
	int counter = 0;
	char* lstr = *str;
	char c = lstr[counter];
	while (is_number(c))
	{
		buff[counter] = c;
		c = lstr[++counter];
	}
	buff[counter] = '\0';
	if (counter != 0) *str = (char*)(((uintptr_t)*str) + (counter - 1));
	int res = atoi(buff);
	return res;
}

char* skip_char(char* str, char c) {
	while (*str == c) str++;
	return str;
}
