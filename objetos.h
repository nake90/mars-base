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
#include "GL/openglut.h"
#include "IL/ilut.h"

/* --- DEFINES --- */
#define MAX_VERTICES 65536 /*!< Max number of vertices - ESTÁ DEFINIDO ASÍ EN EL FORMATO 3ds (Maybe alloc?) */
#define MAX_POLYGONS 65536 /*!< Max number of polygons - ESTÁ DEFINIDO ASÍ EN EL FORMATO 3ds (Maybe alloc?) */


/*! Para más info sobre tamaño y uso ver: "3ds_main_inf.txt"
 - #OK# -> Implementado y funcionando 
 - #FALTA# -> Aun no implementado
 - #UNK# -> No tengo claro para qué funciona ni si lo implementaré
 - #POSIBLE# -> Posiblemente lo implementaré; aun por decidir */

/*! MAIN */
#define MAIN3DS 0x4D4D				/*!< #OK# Main chunk */
/*! MATERIAL */
#define EDIT_MATERIAL 0xAFFF		/*!< #FALTA# 3D Material chunk */
/*! OBJECT */
#define EDIT3DS 0x3D3D				/*!< #OK# 3D Editor chunk */
#define EDIT_OBJECT 0x4000			/*!< #OK# Object block */
#define OBJ_TRIMESH 0x4100			/*!< #OK# Triangular mesh */
#define TRI_VERTEXL 0x4110			/*!< #OK# Vertices list */
#define TRI_VERTEXOPTIONS 0x4111	/*!< #UNK# POINT_FLAG_ARRAY */
#define TRI_FACEL1 0x4120			/*!< #OK# Polygons (faces) list */
#define TRI_MATERIAL 0x4130			/*!< #FALTA# MSH_MAT_GROUP mesh_material_group */
#define TRI_MAPPINGCOORS 0x4140		/*!< #OK# Vertices list */
#define TRI_SMOOTH 0x4150			/*!< #POSIBLE# SMOOTH_GROUP */
#define TRI_LOCAL 0x4160			/*!< #POSIBLE# MESH_MATRIX */
#define TRI_VISIBLE 0x4165			/*!< #POSIBLE# MESH_COLOR */
#define TRI_MAPPINGSTANDARD 0x4170	/*!< #UNK# MESH_TEXTURE_INFO */

/* - TYPEDEFS usados - */
/* Vértices */
/*typedef struct{float x,y,z;}t_vertex; USO VECTOR definido en shared.h */

typedef struct{int a,b,c; int texture;}t_polygon; /*!< Polígono -> Apunta a 3 vértices y a una textura */
typedef struct{float u,v;}t_mapcoord; /*!< Coordenadas u,v de la textura */

/*! Datos del modelo */
typedef struct
{
	char name[80]; /*!< Nombre definido en el archivo 3ds */
	int vertices_qty; /*!< Cantidad de vertices */
	int polygons_qty; /*!< Cantidad de polígonos */
	VECTOR vertex[MAX_VERTICES]; 
	t_polygon polygon[MAX_POLYGONS];
	t_mapcoord mapcoord[MAX_VERTICES];
	float size; /* Valor de la escala */
}t_model, *t_model_ptr;

/*! Base -> Modelo base para usar como ejemplo a la hora de crear más tipos de objetos */
typedef struct
{
	char name[256]; /*!< Nombre del objeto */
	t_model modelo; /*!< Datos de vértices y demás */
	int icono; /*!< ID de la textura usada como icono. (Estilo del spawn_menu del Garry's Mod) */
	VECTOR pos; /*!< Posición (en metros) */
	VECTOR rot; /*!< Pitch, yaw y roll actuales del objeto {Pitch: x; Yaw: y; Roll: z} */
	VECTOR vel; /*!< Velocidad (en m/s) */
	VECTOR velang; /*!< Velocidad angular (en rad/s) */
}t_obj_base, *t_obj_base_ptr;

/* - FUNCIONES - */
int load_3DS (t_model_ptr data, char *filename);
void object_draw(t_obj_base object);

#endif
