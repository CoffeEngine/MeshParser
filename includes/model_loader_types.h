#ifndef CAFFEINE_MESH_LOADER_TYPES_H_
#define CAFFEINE_MESH_LOADER_TYPES_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct {
	float x, y, z;
} vec3;

typedef struct {
	float x, y;
} vec2;

typedef struct {
	float x, y, z, w;
} vec4;

typedef struct {
	int position_index, texture_index, normal_index;
} VertexIndex;

typedef struct {
	vec3 position;
	vec3 normal;
	vec2 uv;
} Vertex;

typedef struct {
	unsigned int a, b, c;
} Face;

typedef struct {
	unsigned int num_vertex;
	unsigned int num_faces;

	Vertex* vertex;
	Face* faces;
} Model;

#endif