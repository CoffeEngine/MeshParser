#include "model_loader.h"
#include <stdint.h>

void model_loader_free(Model* ogl) {
	free(ogl->faces);
	free(ogl->vertex);
}

void model_loader_from_text_file(Model* model, const char* const file_path) {
	FILE* nfile;
	errno_t e = fopen_s(&nfile, file_path, "r");

	if (e) {
		fprintf(stderr, "Fail to open file. err: %d\n", e);
		return;
	}

	fscanf(nfile, "%d %d\n\n", &model->num_vertex, &model->num_faces);

	model->faces = (Face*)malloc(sizeof(Face) * model->num_faces);
	if (model->faces == NULL) exit(-1);
	model->vertex = (Vertex*)malloc(sizeof(Vertex) * model->num_vertex);
	if (model->vertex == NULL) {
		free(model->faces);
		exit(-1);
	}


	for (uint32_t i = 0; i < model->num_vertex; i++)
	{
		Vertex* v = model->vertex + i;
		fscanf(nfile, "%f %f %f %f %f %f %f %f\n", &v->position.x, &v->position.y, &v->position.z, &v->normal.x, &v->normal.y, &v->normal.z, &v->uv.x, &v->uv.y);
	}

	fscanf(nfile, "\n");

	for (uint32_t i = 0; i < model->num_faces; i++)
	{
		Face* f = model->faces + i;
		fscanf(nfile, "%u %u %u\n", &f->a, &f->b, &f->c);
	}

	fclose(nfile);
}

void model_loader_from_bin_file(Model* model, const char* const file_path) {
	FILE* nfile;
	errno_t e = fopen_s(&nfile, file_path, "rb");

	if (e) {
		fprintf(stderr, "Fail to open file. err: %d\n", e);
		return;
	}

	fread(&(model->num_vertex), sizeof(uint32_t), 1, nfile);
	fread(&(model->num_faces), sizeof(uint32_t), 1, nfile);

	model->faces = (Face*)malloc(sizeof(Face) * model->num_faces);
	if (model->faces == NULL) exit(-1);
	model->vertex = (Vertex*)malloc(sizeof(Vertex) * model->num_vertex);
	if (model->vertex == NULL) {
		free(model->faces);
		exit(-1);
	}

	fread(model->vertex, sizeof(Vertex), model->num_vertex, nfile);
	fread(model->faces, sizeof(Face), model->num_faces, nfile);

	fclose(nfile);
}

void model_loader_from_text_file_f(Model* model, FILE* file_ptr) {

	fscanf(file_ptr, "%d %d\n\n", &model->num_vertex, &model->num_faces);

	model->faces = (Face*)malloc(sizeof(Face) * model->num_faces);
	if (model->faces == NULL) exit(-1);
	model->vertex = (Vertex*)malloc(sizeof(Vertex) * model->num_vertex);
	if (model->vertex == NULL) {
		free(model->faces);
		exit(-1);
	}


	for (uint32_t i = 0; i < model->num_vertex; i++)
	{
		Vertex* v = model->vertex + i;
		fscanf(file_ptr, "%f %f %f %f %f %f %f %f\n", &v->position.x, &v->position.y, &v->position.z, &v->normal.x, &v->normal.y, &v->normal.z, &v->uv.x, &v->uv.y);
	}

	fscanf(file_ptr, "\n");

	for (uint32_t i = 0; i < model->num_faces; i++)
	{
		Face* f = model->faces + i;
		fscanf(file_ptr, "%u %u %u\n", &f->a, &f->b, &f->c);
	}

	fclose(file_ptr);
}

void model_loader_from_bin_file_f(Model* model, FILE* file_ptr) {
	
	fread(&(model->num_vertex), sizeof(uint32_t), 1, file_ptr);
	fread(&(model->num_faces), sizeof(uint32_t), 1, file_ptr);

	model->faces = (Face*)malloc(sizeof(Face) * model->num_faces);
	if (model->faces == NULL) exit(-1);
	model->vertex = (Vertex*)malloc(sizeof(Vertex) * model->num_vertex);
	if (model->vertex == NULL) {
		free(model->faces);
		exit(-1);
	}

	fread(model->vertex, sizeof(Vertex), model->num_vertex, file_ptr);
	fread(model->faces, sizeof(Face), model->num_faces, file_ptr);

	fclose(file_ptr);
}
