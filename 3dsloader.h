/*
 * ---------------- www.spacesimulator.net --------------
 *   ---- Space simulators and 3d engine tutorials ----
 *
 *  Author: Damiano Vitulli <info@spacesimulator.net>
 *
 * ALL RIGHTS RESERVED
 *
 *
 * Tutorial 4: 3d engine - 3ds models loader
 * 
 * File header: 3dsloader.h
 *  
 * EDITED by: Alfonso Arbona Gimeno <nake90@terra.es>
 * INFO: http://www.the-labs.com/Blender/3dsspec.html
 */

#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

/* ### NOTA - 3DS es un formato propietario... habría que usar uno libre

INFO DE FORMATO:
Dividido en chunk's
	short chunk_id -> Único y define el tipo y chunks subordinados
	long chunk_len -> Puede ser mayor del necesario si tiene sub-chunks! Omitirlos [EN BYTES]

NOTAR QUE LOS BYTES SE LEEN AL REVÉS: teorico:[AF FF] -> [FF AF]:guardado
Fragmento del 3ds:
            MAIN3DS  (0x4D4D)
            |
            +--EDIT_MATERIAL  (0xAFFF)
            |     |
            |     +--MAT_NAME		(0xA000)
            |     +--AMBIENT_COLOR	(0xA010)
            |     |     |
            |     |     +--RGB1	(0x0011) [En teoría son lo mismo... usamos solo el 1 y au!]
            |     |     +--RGB2	(0x0012)
            |     |
            |     +--DIFFUSE_COLOR	(0xA020) RGB's...
            |     |     *
            |     |
            |     +--SPECULAR_COLOR	(0xA030) RGB's...
            |     +--SHININESS		(0xA040)
            |     |     |
            |     |     +--AMOUNT		(0x0030) [short integer]
            |     |     +--DISPLAYED	(0xA252) [No lo uso]
            |     |
            |     +--SHIN_STRENGTH	(0xA041) AMOUNT..
            |     |     *
            |     |
            |     +--TRANSPARENCY	(0xA050) AMOUNT..
            |     +--TRANS_FALLOFF	(0xA052) AMOUNT..
            |     +--REFLECT_BLUR	(0xA053) AMOUNT..
            |     +--MATERIAL_TYPE	(0xA100) [1=flat 2=gour. 3=phong 4=metal]
            |     +--SELF_ILLUM		(0xA084) AMOUNT..
            |     |
            |	  *
            |	  *
            |
			+--EDIT3DS  (0x3D3D)
			|     |
            |     +--EDIT_OBJECT   (0x4000)
            |     |  |  |
            |     |  |  +--OBJ_TRIMESH   (0x4100)      
            |     |  |  |  |
            |     |  |  |  +--TRI_VERTEXL          (0x4110) 
            |     |  |  |  +--TRI_VERTEXOPTIONS    (0x4111)
            |     |  |  |  +--TRI_MAPPINGCOORS     (0x4140) 
            |     |  |  |  +--TRI_MAPPINGSTANDARD  (0x4170)
            |     |  |  |  +--TRI_FACEL1           (0x4120)
            |     |  |  |  |  |
            |     |  |  |  |  +--TRI_SMOOTH            (0x4150)   
            |     |  |  |  |  +--TRI_MATERIAL          (0x4130)
            |     |  |  |  |
            |     |  |  |  +--TRI_LOCAL            (0x4160)
            |     |  |  |  +--TRI_VISIBLE          (0x4165)
			*	  *	 *	*
			*	  *	 *	*
			*	  *	 *	*
			
*/

/* MAIN */
#define MAIN3DS 0x4D4D			/*OK Main chunk, contains all the other chunks - Lenght: 0 + sub chunks */
/* MATERIAL */
#define EDIT_MATERIAL 0xAFFF	/* #FALTA# 3D Editor chunk, objects layout info - Lenght: 0 + sub chunks */

/* OBJECT */
#define EDIT3DS 0x3D3D			/*OK 3D Editor chunk, objects layout info - Lenght: 0 + sub chunks */
#define EDIT_OBJECT 0x4000		/*OK Object block, info for each object - Lenght: len(object name) + sub chunks */
#define OBJ_TRIMESH 0x4100		/*OK Triangular mesh, contains chunks for 3d mesh info - Lenght: 0 + sub chunks */
#define TRI_VERTEXL 0x4110		/*OK Vertices list
								Lenght:
									1 x unsigned short (number of vertices)
									+ 3 x float (vertex coordinates) x (number of vertices)
									+ sub chunks */
#define TRI_VERTEXOPTIONS 0x4111/* #UNK# POINT_FLAG_ARRAY - short nflags; short flags[nflags];  */
#define TRI_FACEL1 0x4120		/*OK Polygons (faces) list
								Lenght:
									1 x unsigned short (number of vertices)
									+ 3 x unsigned short (polygon points) x (number of polygons)
									+ sub chunks */
#define TRI_MATERIAL 0x4130		/* #FALTA# MSH_MAT_GROUP mesh_material_group - cstr material_name; short nfaces; short facenum[nfaces]; */
#define TRI_MAPPINGCOORS 0x4140	/*OK Vertices list
								Lenght:
									1 x unsigned short (number of mapping points)
									+ 2 x float (mapping coordinates) x (number of mapping points)
									+ sub chunks */
#define TRI_SMOOTH 0x4150		/* #POSIBLE# SMOOTH_GROUP - short grouplist[n]; (determined by length, seems to be 4 per face) */
#define TRI_LOCAL 0x4160		/* #POSIBLE# MESH_MATRIX - float matrix[4][3]; */
#define TRI_VISIBLE 0x4165		/* #POSIBLE# MESH_COLOR - short color_index; */
#define TRI_MAPPINGSTANDARD 0x4170		/* #UNK# MESH_TEXTURE_INFO - short map_type; float x_tiling, y_tiling; float icon_x, icon_y, icon_z; float matrix[4][3]; float scaling, plan_icon_w, plan_icon_h, cyl_icon_h;  */



/**********************************************************
 *
 * FUNCTION Load3DS (obj_type_ptr, char *)
 *
 * This function loads a mesh from a 3ds file.
 * Please note that we are loading only the vertices, polygons and mapping lists.
 * If you need to load meshes with advanced features as for example: 
 * multi objects, materials, lights and so on, you must insert other chunk parsers.
 *
 *********************************************************/


#include "shared.h"

#define MAX_VERTICES 65536 /* Max number of vertices - ESTÁ DEFINIDO ASÍ EN EL FORMATO 3ds */
#define MAX_POLYGONS 65536 /* Max number of polygons - ESTÁ DEFINIDO ASÍ EN EL FORMATO 3ds */
#define DEBUG_LOAD 0

// Our vertex type
typedef struct{
    float x,y,z;
}vertex_type;

// The polygon (triangle), 3 numbers that aim 3 vertices
typedef struct{
    int a,b,c;
}polygon_type;

// The mapcoord type, 2 texture coordinates for each vertex
typedef struct{
    float u,v;
}mapcoord_type;

// The object type
typedef struct {
	char name[20];
    
	int vertices_qty;
    int polygons_qty;

    vertex_type vertex[MAX_VERTICES]; 
    polygon_type polygon[MAX_POLYGONS];
    mapcoord_type mapcoord[MAX_VERTICES];
    int id_texture;
    float size;
    float pitch;
    float yaw;
    float roll;
    float x,y,z;
    float vx,vy,vz;
} t_3ds_model, *t_3ds_model_ptr;

extern char Load3DS (t_3ds_model_ptr ogg, char *filename);

#endif
