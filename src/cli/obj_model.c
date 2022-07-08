#include "obj_model.h"


vec3 parse_position(const char* const str) {
	vec3 v;
	assert(sscanf(str, "v %f %f %f", &v.x, &v.y, &v.z) == 3);
	return v;
}

vec2 parse_texture_coord(const char* const str) {
	vec2 v;
	assert(sscanf(str, "vt %f %f", &v.x, &v.y) == 2);
	return v;
}

vec3 parse_normal(const char* const str) {
	vec3 v;
	assert(sscanf(str, "vn %f %f %f", &v.x, &v.y, &v.z) == 3);
	return v;
}

size_t parse_face(const char* str, VertexIndex* f, int32_t triangulate) {

	int face_index = 0;
	int face_prop = 0;

	str = skip_char(str, 'f'); //skip f char
	str = skip_char(str, ' '); // skip blank char

	char c = str[0];

	while (c != '\n' && c != '\0')
	{
		if (face_index == 4) break;
		if (is_number(c)) {
			int pos = read_int(&str) - 1;
			switch (face_prop)
			{
			case 0:
				f[face_index].position_index = pos;
				break;
			case 1:
				f[face_index].texture_index = pos;
				break;
			case 2:
				f[face_index].normal_index = pos;
				break;
			}
		}
		else if (c == '/') {
			face_prop++;
		}
		else if (c == ' ') {
			face_index++;
			face_prop = 0;
			str = skip_char(str, ' ');
			c = str[0];
			continue;
		}


		c = *(++str);
		if (c == '\n') face_index++;
	}

	if (face_index == 4 && triangulate) { //triangulate
		f[4] = f[3];
		f[3] = f[2];
		f[5] = f[0];
		face_index = 6;
	}

	return face_index;
}



void init_obj_model(ObjModel* data) {
	const uint32_t vertex_len = 32;
	const uint32_t index_len = 48;

	data->count_normals = 0;
	data->count_positions = 0;
	data->count_uvs = 0;
	data->count_index = 0;

	data->len_normals = vertex_len;
	data->len_positions = vertex_len;
	data->len_uvs = vertex_len;
	data->len_index = index_len;

	data->positions = (vec3*)malloc(sizeof(vec3) * vertex_len);
	data->normals = (vec3*)malloc(sizeof(vec3) * vertex_len);
	data->uvs = (vec2*)malloc(sizeof(vec2) * vertex_len);
	data->index = (VertexIndex*)malloc(sizeof(VertexIndex) * index_len);

	data->has_normal = false;
	data->has_texture = false;
}

void obj_model_add_position(ObjModel* data, vec3 pos) {
	if (data->count_positions == data->len_positions) {
		uint32_t nlen = data->len_positions * 2;
		vec3* nptr = realloc(data->positions, sizeof(vec3) * nlen);
		if (nptr) {
			data->len_positions = nlen;
			data->positions = nptr;
		}
		else
		{
			exit(-1);
		}
	}
	data->positions[data->count_positions] = pos;
	data->count_positions++;
}

void obj_model_add_normal(ObjModel* data, vec3 normal) {
	if (data->count_normals == data->len_normals) {
		uint32_t nlen = data->len_normals * 2;
		vec3* nptr = realloc(data->normals, sizeof(vec3) * nlen);
		if (nptr) {
			data->len_normals = nlen;
			data->normals = nptr;
		}
		else
		{
			exit(-1);
		}
	}
	data->normals[data->count_normals] = normal;
	data->count_normals++;
	data->has_normal = true;
}

void obj_model_add_texture(ObjModel* data, vec2 texture) {
	if (data->count_uvs == data->len_uvs) {
		uint32_t nlen = data->len_uvs * 2;
		vec2* nptr = realloc(data->uvs, sizeof(vec2) * nlen);
		if (nptr) {
			data->len_uvs = nlen;
			data->uvs = nptr;
		}
		else
		{
			exit(-1);
		}
	}
	data->uvs[data->count_uvs] = texture;
	data->count_uvs++;
	data->has_texture = true;
}

void obj_model_add_face(ObjModel* data, VertexIndex face) {
	if (data->count_index == data->len_index) {
		uint32_t nlen = data->len_index * 2;
		VertexIndex* nptr = realloc(data->index, sizeof(VertexIndex) * nlen);
		if (nptr) {
			data->len_index = nlen;
			data->index = nptr;
		}
	}
	data->index[data->count_index] = face;
	data->count_index++;
}

void obj_model_normalize(ObjModel* data) {
	uint32_t index_c = data->count_index;
	for (size_t i = 0; i < index_c; i++)
	{
		if (data->index[i].normal_index < 0) data->index[i].normal_index += index_c;
		if (data->index[i].position_index < 0) data->index[i].position_index += index_c;
		if (data->index[i].texture_index < 0) data->index[i].texture_index += index_c;

		if (!data->has_normal)data->index[i].normal_index = 0;
		if (!data->has_texture)data->index[i].texture_index = 0;
		
	}
}

void obj_model_free(ObjModel* data) {
	if (data == NULL) return;
	if (data->positions != NULL) free(data->positions);
	if (data->normals != NULL) free(data->normals);
	if (data->uvs != NULL) free(data->uvs);
	if (data->index != NULL) free(data->index);
}

void obj_model_to_model(ObjModel* obj, Model* model) {
	VextexHash vhash;

	uint32_t v_len = obj->count_positions, v_count = 0;
	uint32_t f_len = obj->count_index, f_count = 0;

	Vertex* vertex = (Vertex*)malloc(sizeof(Vertex) * v_len);
	uint32_t* indexes = (uint32_t*)malloc(sizeof(uint32_t) * f_len);

	vertex_hash_init(&vhash, obj->count_index);

	for (size_t i = 0; i < obj->count_index; i++)
	{
		if (v_count == v_len) {
			uint32_t nlen = v_len * 2;
			Vertex* nptr = realloc(vertex, nlen * sizeof(Vertex));
			if (nptr) {
				v_len = nlen;
				vertex = nptr;
			}
			else exit(-1);
		}

		if (f_count == f_len) {
			uint32_t nlen = f_len * 2;
			uint32_t* nptr = (uint32_t*)realloc(indexes, sizeof(uint32_t) * nlen);
			if (nptr) {
				f_len = nlen;
				indexes = nptr;
			}
			else exit(-1);
		}

		VertexIndex vi = obj->index[i];
		Vertex v;
		v.position = obj->positions[vi.position_index];
		v.normal = obj->has_normal ? obj->normals[vi.normal_index] : (vec3) { 0, 0, 0 };
		v.uv = obj->has_texture ? obj->uvs[vi.texture_index] : (vec2) { 0, 0 };

		uint32_t vertex_index = 0;
		if (vertex_hash_get(&vhash, vi, &vertex_index)) {
			indexes[f_count++] = vertex_index;
		}
		else {
			vertex_hash_add(&vhash, vi, v_count);
			vertex[v_count] = v;
			indexes[f_count++] = v_count;
			v_count++;
		}
	}

	vertex_hash_free(&vhash);

	{
		void* tmp = realloc(vertex, v_count * sizeof(Vertex));
		if (tmp != NULL) vertex = tmp;
		else exit(-1);
	}

	{
		void* tmp = realloc(indexes, f_count * sizeof(uint32_t));
		if (tmp != NULL) indexes = tmp;
		else exit(-1);
	}

	model->num_faces = f_count / 3;
	model->num_vertex = v_count;
	model->vertex = vertex;
	model->faces = (Face*)indexes;
}

void obj_model_from_file(FileScanner* scn, int32_t triangulate, void* ctx, on_loaded_callback on_loaded) {
	ObjModel obj_data = { 0 };
	vec3 pos = { 0 }, norm = { 0 };
	vec2 uv = { 0 };
	VertexIndex faces[6] = { 0 };
	size_t count = 0;
	init_obj_model(&obj_data);

	while (file_scanner_next_line(scn))
	{
		switch (scn->linebuffer[0]) {
		case 'v':
			switch (scn->linebuffer[1]) {
			case ' ':
				pos = parse_position(scn->linebuffer);
				obj_model_add_position(&obj_data, pos);
				break;
			case 't':
				uv = parse_texture_coord(scn->linebuffer);
				obj_model_add_texture(&obj_data, uv);
				break;
			case 'n':
				norm = parse_normal(scn->linebuffer);
				obj_model_add_normal(&obj_data, norm);
				break;
			}
			break;
		case 'f':

			count = parse_face(scn->linebuffer, faces, triangulate);
			for (size_t i = 0; i < count; i++)
			{

				if (faces[i].position_index < 0) {
					faces[i].position_index += obj_data.count_positions + 1;
				}

				if (faces[i].texture_index < 0) {
					faces[i].texture_index += obj_data.count_uvs + 1;
				}

				if (faces[i].normal_index < 0) {
					faces[i].normal_index += obj_data.count_normals + 1;
				}

				obj_model_add_face(&obj_data, faces[i]);
			}
			break;
		}
	}

	obj_model_normalize(&obj_data);
	on_loaded(&obj_data, ctx);
	obj_model_free(&obj_data);
}