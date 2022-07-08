#ifndef _CAFFEINE_MODEL_H_
#define _CAFFEINE_MODEL_H_

#include "common.h"

void model_print(Model* model);
void model_to_text_file(Model* model, const char* const filename);
void model_to_bin_file(Model* model, const char* const filename);
void model_free(Model* model);

#endif // !_CAFFEINE_MODEL_H_
