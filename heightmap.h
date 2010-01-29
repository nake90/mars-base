/*
	mars_base - Design, build and maintain your own base on Mars
    Copyright (C) 2009  Alfonso Arbona Gimeno (nake90@terra.es). All rights reserved.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    If you use any part of this code you must give me (Alfonso Arbona Gimeno) credit.
    If you plan to use any part of this code on a comercial game please email me at:
	   	   nake90@terra.es
*/

/** \file heightmap.h
 * \brief Header que controla los heightmaps (mapas)
 * \author Alfonso Arbona Gimeno
*/

#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H
#include "shared.h"
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <SDL_ttf.h>
#include <IL/ilut.h>

#define NHMAP_VERSION 2 /* Define la versión de los mapas precompilados */

#define AMBIENTE 0.2f
#define MAP_Z_OFFSET 0.20
#define pos_a_casilla_x(m,x) ((int)((m).tam_x -((m).tam_x -((m).tam_x/2) -((x)+(m).ini_x)/(m).scale)+1))
#define pos_a_casilla_y(m,y) ((int)((m).tam_y -(((m).tam_y/2) +((y)+(m).ini_y)/(m).scale) -1))

typedef struct
{
	GLuint list; /* list -> Terreno; list+1 -> Casillas; list+2 -> Normales; */
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
	unsigned char* data; /* Alturas (0-255) */
}t_heightmap;
t_heightmap marte;

int load_heightmap(const char* filename, t_heightmap* h_buffer, t_texture texture);
void destroy_heightmap(t_heightmap* obj);
void compile_map(t_heightmap* obj, t_texture texture);
void save_compiled_map(const char* ruta, t_heightmap obj);
int load_compiled_map(const char* ruta, t_heightmap* obj, t_texture texture);
float calc_shadow (int obj_x, int obj_y, VECTOR ray, t_heightmap* obj, float ambiente);
void list_compile_map(t_heightmap* obj, t_texture texture);

float get_real_height(t_heightmap obj, float coord_x, float coord_y);
float z_to_real_height (t_heightmap obj, int z);
float coord_to_real_height(t_heightmap obj, float z);

int get_current_triangle (t_heightmap obj, float coord_x, float coord_y, VECTOR *v1, VECTOR *v2, VECTOR *v3);

#endif
