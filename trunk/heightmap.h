#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H
#include "shared.h"
#include <stdio.h>
#include "GL/openglut.h"
#include "IL/ilut.h"

#define AMBIENTE 0.2f

typedef struct
{
	GLuint list;
	int tam_x;
	int tam_y;
	float scale;
	int zero_h;
	int min_h;
	int max_h;
	float north;
	float south;
	float east;
	float west;
	int ini_x;
	int ini_y;
	int ini_z;
	VECTOR* normal; /* Vector normal al plano (unitario) */
	float* shadow; /* Valor de gris (0-1) */
	int* data; /* Alturas (0-255) */
}t_heightmap;

int load_heightmap(const char* filename, t_heightmap* h_buffer, t_texture texture);
void destroy_heightmap(t_heightmap* obj);
void compile_map(t_heightmap* obj, t_texture texture);
void save_compiled_map(const char* ruta, t_heightmap obj);
int load_compiled_map(const char* ruta, t_heightmap* obj, t_texture texture);
float calc_shadow (int obj_x, int obj_y, VECTOR ray, t_heightmap* obj, float ambiente);

#endif
