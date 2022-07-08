#include "common.h"
#include "file_scanner.h"
#include "obj_model.h"


/*
 0 params: show help text

 -h | help: show help text

 1 param: path to file

 -o: output file name
 -f: format (b|t)

*/

void show_help(void) {
	printf("Program usage:\n");
	printf("\tmeshc | meshc -h | meshc -help\t\tShow help text.\n");
	printf("\tmeshc [path]\t\tCompiles the mesh especified in [path]. Ex: mesch myfile.obj\n");
	printf("\t-o\t\tEspecify output file. Ex: mesch myfile.obj -o compiled.bin\n");
	printf("\t-f\t\tEspecify output file format, accepts b|B to binary format and t|T to text format. Default value: b. Ex: mesch myfile.obj -f t\n");
}

int check_flag(char* flag, const char* value) {
	
	return strcmp(flag, value) == 0;
}

char* get_filename(char* path) {
	char* tmp = strrchr(path, '\\');
	if (tmp == NULL) return path;
	return tmp+1;
}

void change_extension(char* path, const char* ext,size_t max_len) {
	char* tmp = strrchr(path, '.');
	
	if (tmp == NULL) {
		tmp = path+ strlen(path);
		*tmp = '.';
	}
	tmp++;
	
	size_t l = strlen(tmp);
	size_t ext_len = strlen(ext);

	for (size_t i = 0; i < ext_len; i++)
	{
		if (l == max_len) break;
		*tmp = ext[i];
		tmp++; 
		l++;
	}
	*tmp = '\0';
}

void on_loaded(ObjModel* obj, Model* ctx) {
	obj_model_to_model(obj, ctx);
}

void parse(char* from, char* to, int binary) {
	FileScanner scn = create_file_scanner(from, 0);

	if (scn.file_content == NULL) {
		printf("Failed to create scanner\n");
		exit(-1);
	}

	Model ogl = { 0 };

	obj_model_from_file(&scn, true, &ogl, on_loaded);

	if(binary == 1)
		model_to_bin_file(&ogl, to);
	else
		model_to_text_file(&ogl, to);

	model_free(&ogl);

	destroy_file_scanner(&scn);
}

int main(int argc,char** argv) {


	if (argc == 1 || check_flag(argv[1],"-h") || check_flag(argv[1], "-help")) {
		show_help();
		return 0;
	}


	//file path always is the first input except for help flag
	const char* file_path = argv[1];
	char* file_output = NULL,*gen_name = NULL;
	unsigned int binary_format = 1;
	unsigned int verbose = 0;



	for (size_t i = 2; i < argc; i+=1)
	{
		if (check_flag(argv[i], "-o")) {
			file_output = argv[i + 1];
			i++;
		}

		if (check_flag(argv[i], "-f")) {
			binary_format = !check_flag(argv[i + 1], "t");
			i++;
		}

		if (check_flag(argv[i], "-v")) {
			verbose = 1;
		}
	}


	if (file_output == NULL) {
		char* filename = get_filename(file_path);
		size_t size = ((size_t)strlen(filename))+6;
		gen_name = malloc(size);
		if (gen_name == NULL) {
			printf("Failed to allocate memory\n");
			exit(-1);
		}
		memset(gen_name, '\0', size);
		strncpy(gen_name, filename, size-6);
		

		if (binary_format) {
			change_extension(gen_name, "bin", size-1);
		}
		else {
			change_extension(gen_name, "cff", size-1);
		}

		file_output = gen_name;
	}

	if (verbose) {
		printf("FROM: %s\n", file_path);
		printf("TO: %s\n", file_output);
		printf("FORMAT: %s\n", binary_format?"binary":"text");
	}
	

	parse(file_path, file_output, binary_format);

	if (gen_name != NULL) {
		free(gen_name);
	}
}