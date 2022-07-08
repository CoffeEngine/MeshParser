#include "model.h"
#include <stdio.h>



void model_print(Model* model) {
	printf("Num Vextex: %d\nNum Index:%d\n\n", model->num_vertex, model->num_faces);
	for (uint32_t i = 0; i < model->num_vertex; i++)
	{
		Vertex v = model->vertex[i];
		printf("%u  v: %.4f, %.4f, %.4f ; n: %.4f, %.4f, %.4f ; t: %.4f, %.4f\n", i, v.position.x, v.position.y, v.position.z, v.normal.x, v.normal.y, v.normal.z, v.uv.x, v.uv.y);
	}
	printf("\n\n");
	for (uint32_t i = 0; i < model->num_faces; i++)
	{
		printf("%u, %u, %u\n", model->faces[i].a, model->faces[i].b, model->faces[i].c);

	}
}

void model_to_text_file(Model* model, const char* const filename) {
	FILE* nfile;
	errno_t e = fopen_s(&nfile,filename, "w");

	if (e) {
		fprintf(stderr, "Fail to open file. err: %d\n", e);
		return;
	}

	fprintf(nfile,"%d %d\n\n", model->num_vertex, model->num_faces);
	
	for (uint32_t i = 0; i < model->num_vertex; i++)
	{
		Vertex v = model->vertex[i];
		fprintf(nfile,"%f %f %f %f %f %f %f %f\n", v.position.x, v.position.y, v.position.z, v.normal.x, v.normal.y, v.normal.z, v.uv.x, v.uv.y);
	}
	
	fprintf(nfile,"\n");

	for (uint32_t i = 0; i < model->num_faces; i++)
	{
		fprintf(nfile,"%u %u %u\n", model->faces[i].a, model->faces[i].b, model->faces[i].c);
	}

	fclose(nfile);
}

void model_to_bin_file(Model* model, const char* const filename) {
	FILE* nfile;
	errno_t e = fopen_s(&nfile, filename, "wb");

	if (e) {
		fprintf(stderr, "Fail to open file. err: %d\n", e);
		return;
	}

	fwrite(&model->num_vertex, sizeof(uint32_t), 1, nfile);
	fwrite(&model->num_faces, sizeof(uint32_t), 1, nfile);
	
	fwrite(model->vertex, sizeof(Vertex), model->num_vertex, nfile);
	fwrite(model->faces, sizeof(Face), model->num_faces, nfile);
	
	fclose(nfile);
}

void model_free(Model* model) {
	if (model == NULL) return;
	if(model->faces != NULL) free(model->faces);
	if (model->vertex != NULL) free(model->vertex);
}