/*
	Copyright (C) 2009 Alfonso Arbona Gimeno (nake90@terra.es)
	
	This file is part of mars_base.

    Mars_base is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Mars_base is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

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
