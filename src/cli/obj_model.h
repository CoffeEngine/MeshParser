#ifndef _CAFFEINE_OBJ_MODEL_H_
#define _CAFFEINE_OBJ_MODEL_H_

#include "common.h"
#include "model.h"
#include "file_scanner.h"
#include "utils.h"
#include "hash.h"

typedef struct {
	vec3* positions;
	vec3* normals;
	vec2* uvs;

	VertexIndex* index;

	uint32_t count_positions, count_normals, count_uvs, count_index;
	uint32_t len_positions, len_normals, len_uvs, len_index;

	bool has_normal, has_texture;
} ObjModel;

typedef void (*on_loaded_callback)(ObjModel*, void*);

void init_obj_model(ObjModel* data);
void obj_model_add_position(ObjModel* data, vec3 pos);
void obj_model_add_normal(ObjModel* data, vec3 normal);
void obj_model_add_texture(ObjModel* data, vec2 texture);
void obj_model_add_face(ObjModel* data, VertexIndex face);
void obj_model_normalize(ObjModel* data);
void obj_model_free(ObjModel* data);

void obj_model_to_model(ObjModel* obj, Model* model);

void obj_model_from_file(FileScanner* scn, int32_t triangulate, void* ctx, on_loaded_callback on_loaded);

#endif // !_CAFFEINE_OBJ_MODEL_H_
