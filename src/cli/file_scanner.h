#ifndef _CAFFEINE_FILE_SCANNER_H_
#define _CAFFEINE_FILE_SCANNER_H_

#include "common.h"

typedef struct {
	char* file_content;
	char* linebuffer;
	uint64_t file_size;
	uint64_t cursor_pos;
	uint64_t linebuffer_len;
	uint64_t linebuffer_count;
}FileScanner;

FileScanner create_file_scanner(const char* filepath, uint64_t buffer_lenght);
size_t file_scanner_next_line(FileScanner* scn);
void destroy_file_scanner(FileScanner* scn);

#endif // !_CAFFEINE_FILE_SCANNER_H_