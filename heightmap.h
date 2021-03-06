/*
	mars_base - Design, build and admin your own base on Mars
    Copyright (C) 2009  Alfonso Arbona Gimeno (nake90@terra.es). All rights reserved.

	MIT LICENSE

	Permission is hereby granted, free of charge, to any
	person obtaining a copy of this software and associated
	documentation files (the "Software"), to deal in the
	Software without restriction, including without limitation
	the rights to use, copy, modify, merge, publish,
	distribute, sublicense, and/or sell copies of the
	Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice
	shall be included in all copies or substantial portions of
	the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
	KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
	WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
	PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
	OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
	OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
	OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

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

#define NHMAP_VERSION 2 /*! Define la versi�n de los mapas precompilados */

#define AMBIENTE 0.2f
#define pos_a_casilla_x(m,x) ((int)((m).tam_x -((m).tam_x -((m).tam_x/2) -((x)+(m).ini_x)/(m).scale)+1))
#define pos_a_casilla_y(m,y) ((int)((m).tam_y -(((m).tam_y/2) +((y)+(m).ini_y)/(m).scale) -1))
#define MAP_DETAIL_RANDOM 0 /*!< Cantidad de iteraciones extra que se hacen de forma aleatoria */

#define MAP_RANDOM_ENABLE 0
#define MAP_RANDOM_SUBDIVISIONS 16.0f /*!< Cantidad de subdivisiones aleatorias */
#define MAP_RANDOM_VDIVIDE 16.0f /*!< Valor por el que dividir el v_scale para obtener las alturas m�ximas de los randoms */

typedef struct s_map_vertex
{
	double altura;
	float shadow;
	int textura; //!< Id de la textura a usar.
	// Para a�adir ciclos d�a/noche (se calcula con el producto escalar cada hora o as� y se guarda en shadow)
	//float day_time; //! Define la hora a partir de la cu�l se debe controlar la luz
	//float night_time; //! Momento en el que siempre es de noche (sombra al m�ximo)
}t_map_vertex;

/*
typedef struct s_map_quad
{
	union //!< Id al v�rtice si el quad dentro de max_detail, si no el seed para generar el terreno aleatorio.
	{
		unsigned int vertex[4]; //!< 0: Arriba izquierda, 1: Abajo izquierda, 2: Abajo derecha, 3: Arriba derecha (De aqu� saco las coordenadas x,y)
		struct s_rand_quad
		{
			float seed;
			int x, y, lado;
		}rand_quad;
	};
	VECTOR normal[2]; //!< Vector normal de cada cara
	unsigned int child[4]; //!< Id a los hijos. 0 si no tiene m�s hijos o est�n fuera del mapa.
	unsigned int parent;
	unsigned int vecino[4]; // Up, left, down, right
	unsigned int level;
	//unsigned char modo;
}t_map_quad;
*/

//#define __OLD_NMAP_MODE__
typedef struct
{
    unsigned int list; /* list -> Terreno; list+1 -> Casillas; list+2 -> Normales; */
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
#ifdef __OLD_NMAP_MODE__
    //! ANTIGUO
    VECTOR* normal; /* Vector normal al plano (unitario) */
    float* shadow; /* Valor de gris (0-1) */
    unsigned char* data; /* Alturas (0-255) */
#else
    //! NUEVO
    unsigned int max_detail; //!< M�ximo nivel de detalle (aka: nivel de hijos) con datos reales del mapa.
    //unsigned int max_detail_rand; //!< M�ximo nivel de detalle + los generados de forma aleatoria.
    //int pow_mult_scale; //!< Controla las unidades del mapa, usado para ampliar el mapa si hay randoms para que el tama�o de las casillas sea 1
		t_map_vertex *vertex; //!< Datos de los v�rtices guardados a saco para ser referenciados (No tiene por qu� estar en orden).
		//t_map_quad *quadtree; //!< Lista de quadtrees. El inicial es el �ltimo de la lista, el resto se referencian.
		//unsigned int num_quads; //!< Cantidad de quadtrees, la f�rmula es complicada por eso lo guardo.
		//unsigned int num_parents; //!< Cantidad de quads que son iniciales, siempre 1 excepto si el mapa no es cuadrado
		float seed;
#endif
}t_heightmap;
extern t_heightmap marte;

int load_heightmap(const char* filename, t_heightmap* h_buffer, t_texture texture); //!! ToDo: Quitar texture!
void destroy_heightmap(t_heightmap* obj);
void compile_map(t_heightmap* obj, t_texture texture);
void save_compiled_map(const char* ruta, t_heightmap obj);
int load_compiled_map(const char* ruta, t_heightmap* obj, t_texture texture);
float calc_shadow (int obj_x, int obj_y, VECTOR ray, t_heightmap* obj, float ambiente);
void list_compile_map(t_heightmap* obj, t_texture texture);

float get_real_height(t_heightmap obj, float coord_x, float coord_y);
float z_to_real_height (t_heightmap obj, int z);
#define altura_al_suelo(m,x,y,z) ((z) - get_real_height((m), (x), (y)))

int get_current_triangle(t_heightmap obj, float coord_x, float coord_y, VECTOR *v1, VECTOR *v2, VECTOR *v3);

int get_traced_coord(VECTOR pos, VECTOR dir, VECTOR *coord);

void quad_dump_data(const char *filename);

#endif
