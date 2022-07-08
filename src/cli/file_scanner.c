#include "file_scanner.h"

FileScanner create_file_scanner(const char* filepath, uint64_t buffer_lenght) {

	FileScanner scn = { 0 };

	if (filepath == NULL) {
		printf("File path is NULL\n");
		return scn;
	}
	FILE* file;
	errno_t e = fopen_s(&file, filepath, "r");

	if (e) {
		printf("Fail to open file. err: %d\n", e);
		return scn;
	}

	fseek(file, 0L , SEEK_END);
	scn.file_size = ftell(file);
	fseek(file, 0L, SEEK_SET);

	if (scn.file_size == 0) {
		printf("File content is empty\n");
		return (FileScanner) { 0 };
	}
	
	if ((scn.file_content = (char*)malloc(sizeof(char) * scn.file_size)) == NULL) {
		printf("Fail to allocate file buffer.\n");
		fclose(file);
		return (FileScanner) { 0 };
	}
	
	fread(scn.file_content, sizeof(char), scn.file_size, file);
	fclose(file);

	if (buffer_lenght <= 0) buffer_lenght = 1024;

	scn.linebuffer_len = buffer_lenght;
	scn.cursor_pos = 0;
	scn.linebuffer_count = 0;

	if ((scn.linebuffer = (char*)malloc(sizeof(char) * scn.linebuffer_len)) == NULL) {
		printf("Fail to allocate file buffer.\n");
		free(scn.file_content);
		return (FileScanner) { 0 };
	}

	return scn;
}

size_t file_scanner_next_line(FileScanner* scn) {
	scn->linebuffer_count = 0;
	char current = '*';
	
	if (scn->cursor_pos == scn->file_size) return 0;

	while ( current != '\n' && (current = scn->file_content[scn->cursor_pos++]) != '\0')
	{
		if (scn->linebuffer_count == scn->linebuffer_len) {
			scn->linebuffer_len *= 2;
			char* tmp = (char*)realloc(scn->linebuffer, scn->linebuffer_len);
			if (tmp == NULL) {
				printf("Failed to allocate memory\n");
				exit(-1);
			}
			scn->linebuffer = tmp;
		}	
		scn->linebuffer[scn->linebuffer_count++] = current;
	}
	scn->linebuffer[scn->linebuffer_count] = '\0';
	return scn->linebuffer_count;
}

void destroy_file_scanner(FileScanner* scn) {
	if (scn == NULL) return;
	if (scn->file_content != NULL) free(scn->file_content);
	if (scn->linebuffer != NULL) free(scn->linebuffer);
}