#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H
#include "shared.h"
#include <stdio.h>
#include "GL/openglut.h"
#include "IL/ilut.h"

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
	int* data;
}t_heightmap;

int load_heightmap(const char* filename, t_heightmap* h_buffer);
void destroy_heightmap(t_heightmap* obj);
void create_map(t_heightmap* obj, t_texture texture);

#endif
