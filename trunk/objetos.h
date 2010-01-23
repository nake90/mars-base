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

/** \file objetos.h
 * \brief Header sobre objetos
 * Este archivo se encarga de todo lo relacionado con los objetos. (Carga, dibujo, control, ...)
 * \author Alfonso Arbona Gimeno
*/

#ifndef OBJETOS_H
#define OBJETOS_H

#include "shared.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <SDL_ttf.h>
#include <IL/ilut.h>

/* --- MACROS --- */
#define obj_setpos(a,b,c,d) a.pos.x=b; a.pos.y=c; a.pos.z=d;

/* --- DEFINES --- */
#define MAX_VERTICES 65536 /*!< Max number of vertices - ESTÁ DEFINIDO ASÍ EN EL FORMATO 3ds (Maybe alloc?) */
#define MAX_POLYGONS 65536 /*!< Max number of polygons - ESTÁ DEFINIDO ASÍ EN EL FORMATO 3ds (Maybe alloc?) */
#define MAX_MATERIALS 256 /*!< Max number of materials (Maybe alloc?) */
#define objetos_debug 0 /*!< Nivel de debug de los objetos */

/*! Para más info sobre tamaño y uso ver: "3ds_main_inf.txt"
 - #OK# -> Implementado y funcionando 
 - #FALTA# -> Aun no implementado
 - #UNK# -> No tengo claro para qué funciona ni si lo implementaré
 - #POSIBLE# -> Posiblemente lo implementaré; aun por decidir
 - #TEST# -> Teoricamente bien, falta testear */

/* MAIN */
#define MAIN3DS 0x4D4D				/*!< #OK# Main chunk */

/* MATERIAL */
#define EDIT_MATERIAL		0xAFFF		/*!< #OK# 3D Material chunk */
#define MAT_NAME 			0xA000		/*!< #OK# Material name ASCII*/
#define MAT_AMBIENT			0xA010		/*!< #OK# Material ambient color SUB_RGB */
#define MAT_DIFFUSE			0xA020		/*!< #OK# Material diffuse color SUB_RGB */
#define MAT_SPECULAR		0xA030		/*!< #OK# Material specular color SUB_RGB */
#define MAT_SHININESS		0xA040		/*!< #OK# Material shininess percent SUB_PER */
#define MAT_SHIN2PCT		0xA041		/*!< #UNK# Material shininess percent SUB_PER */
#define MAT_SHIN3PCT		0xA042		/*!< #UNK# Material shininess percent SUB_PER */
#define MAT_TRANSPARENCY	0xA050		/*!< #OK# Material transparency percent SUB_PER */
#define MAT_XPFALL			0xA052		/*!< #UNK# Material transparency falloff SUB_PER */
#define MAT_REFBLUR			0xA053		/*!< #UNK# Material reflect blur SUB_PER */
#define MAT_TEX1			0xA200		/*!< #OK# Material texture 1 SUB_MAP */
#define MAT_MASK1			0xA33E		/*!< #POSIBLE# Material mask 1 SUB_MAP */
#define MAT_SUB_RGB			0x0011		/*!< #OK# Material SUB_RGB 3chars rgb */
#define MAT_SUB_PER			0x0030		/*!< #OK# Material SUB_PER short integer */
#define MAT_MAPNAME			0xA300		/*!< #OK# Map name (Usado para cargar la textura) ASCII */

/* OBJECT */
#define EDIT3DS 0x3D3D				/*!< #OK# 3D Editor chunk */
#define EDIT_OBJECT 0x4000			/*!< #OK# Object block */
#define OBJ_TRIMESH 0x4100			/*!< #OK# Triangular mesh */
#define TRI_VERTEXL 0x4110			/*!< #OK# Vertices list */
#define TRI_VERTEXOPTIONS 0x4111	/*!< #UNK# POINT_FLAG_ARRAY */
#define TRI_FACEL1 0x4120			/*!< #OK# Polygons (faces) list */
#define TRI_MATERIAL 0x4130			/*!< #OK# MSH_MAT_GROUP mesh_material_group
		cstr material_name;
		short nfaces;
		short facenum[nfaces];*/
#define TRI_MAPPINGCOORS 0x4140		/*!< #OK# Vertices list */
#define TRI_SMOOTH 0x4150			/*!< #POSIBLE# SMOOTH_GROUP */
#define TRI_LOCAL 0x4160			/*!< #POSIBLE# MESH_MATRIX */
#define TRI_VISIBLE 0x4165			/*!< #POSIBLE# MESH_COLOR */
#define TRI_MAPPINGSTANDARD 0x4170	/*!< #UNK# MESH_TEXTURE_INFO */

/* - TYPEDEFS usados - */
typedef struct{int a,b,c; int texture;}t_polygon; /*!< Polígono -> Apunta a 3 vértices y a una textura */
typedef struct{float u,v;}t_mapcoord; /*!< Coordenadas u,v de la textura */

/*! Datos del modelo */
typedef struct
{
	char name[80]; /*!< Nombre definido en el archivo 3ds */
	int vertices_qty; /*!< Cantidad de vertices */
	int polygons_qty; /*!< Cantidad de polígonos */
	int materials_qty; /*!< Cantidad de materiales */
	
	VECTOR vertex[MAX_VERTICES]; 
	t_polygon polygon[MAX_POLYGONS];
	t_mapcoord mapcoord[MAX_VERTICES];
	t_texture material[MAX_MATERIALS];
	
	int draw_list; /*!< Primer índice a la lista de dibujo compilado de OpenGL */
	int draw_lists; /*!< Número de índices compilados de OpenGL */
	
	float size; /* Valor de la escala */
}t_model, *t_model_ptr;

/*! Base -> Modelo base para usar como ejemplo a la hora de crear más tipos de objetos */
typedef struct
{
	char name[256]; /*!< Nombre del objeto */
	t_model_ptr modelo; /*!< Datos de vértices y demás */
	int icono; /*!< ID de la textura usada como icono. (Estilo del spawn_menu del Garry's Mod) */
	VECTOR pos; /*!< Posición (en metros) */
	VECTOR rot; /*!< Pitch, yaw y roll actuales del objeto {Pitch: x; Yaw: y; Roll: z} */
	VECTOR vel; /*!< Velocidad (en m/s) */ /* Mmmmm, voy a usarlos realmente???? los edificios no se mueven... */
	VECTOR velang; /*!< Velocidad angular (en rad/s) */
	float sq_a, sq_b; /*!< Ambos lados que definen la base rectangular de colisión del objeto */
	
}t_obj_base, *t_obj_base_ptr;

/* - FUNCIONES - */
int load_3DS (t_model_ptr data, char *filename);
void object_predraw(t_obj_base_ptr object);
void object_draw(t_obj_base object);
void object_draw_l(t_obj_base_ptr object);
void model_unload(t_model_ptr model);

#endif
