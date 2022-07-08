#ifndef CAFFEINE_MESH_LOADER_H_
#define CAFFEINE_MESH_LOADER_H_

#include "model_loader_types.h"

void model_loader_free(Model* model);

void model_loader_from_text_file(Model* model, const char* const file_path);
void model_loader_from_bin_file(Model* model, const char* const file_path);

void model_loader_from_text_file_f(Model* model, FILE* file_ptr);
void model_loader_from_bin_file_f(Model* model, FILE* file_ptr);

#endif // !CAFFEINE_MESH_LOADER