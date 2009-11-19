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

/** \file objetos.c
 * \brief Control de objetos
 * Este archivo se encarga de todo lo relacionado con los objetos. (Carga, dibujo, control, ...)
 * \author Alfonso Arbona Gimeno
*/

#include "objetos.h"
#include <stdio.h>

/*! \fn int load_3DS (t_model_ptr data, char *filename)
 *  \brief Carga los datos básicos de un objeto desde un archivo 3ds
 *  \param data Pointer al lugar donde se deben guardar los datos
 *  \param filename Ruta al archivo 3ds
 *  \return 0 -> Ok; 1 -> Archivo no encontrado
*/

int load_3DS (t_model_ptr data, char *filename)
{
	int i;
	
	FILE *l_file;
	
	unsigned short l_chunk_id;
	unsigned int l_chunk_lenght;

	unsigned char l_char;
	unsigned short l_qty; /* Número de elementos en el chunk */

	unsigned short l_face_flags; /* Flag that stores some face information */

	if ((l_file=fopen (filename, "rb"))== NULL) return 1;
	
	data->size=1.0f;
	
	while (ftell (l_file) < filelength (fileno (l_file))) /* Leemos todo el archivo */
	{
		fread (&l_chunk_id, 2, 1, l_file); /* HEADER (tipo de chunk) */
		fread (&l_chunk_lenght, 4, 1, l_file); /* Length */

		switch (l_chunk_id)
        {
			case MAIN3DS: /* Lenght: 0 + sub chunks */
			break;    
			
			case EDIT3DS: /* Lenght: 0 + sub chunks */
			break;
			
			case EDIT_OBJECT:
				i=0;
				do
				{
					fread (&l_char, 1, 1, l_file);
                    data->name[i]=l_char;
					i++;
                }while(l_char != '\0' && i<20);
			break;
			
			case OBJ_TRIMESH: /* Lenght: 0 + sub chunks */
			break;
			
			case TRI_VERTEXL:
				fread (&l_qty, sizeof (unsigned short), 1, l_file);
                data->vertices_qty = l_qty;
                for (i=0; i<l_qty; i++)
                {
					fread (&data->vertex[i].x, sizeof(float), 1, l_file);
                    fread (&data->vertex[i].y, sizeof(float), 1, l_file);
					fread (&data->vertex[i].z, sizeof(float), 1, l_file);
				}
				break;
			
			case TRI_FACEL1:
				fread (&l_qty, sizeof (unsigned short), 1, l_file);
                data->polygons_qty = l_qty;
                for (i=0; i<l_qty; i++)
                {
					fread (&data->polygon[i].a, sizeof (unsigned short), 1, l_file);
					fread (&data->polygon[i].b, sizeof (unsigned short), 1, l_file);
					fread (&data->polygon[i].c, sizeof (unsigned short), 1, l_file);
					fread (&l_face_flags, sizeof (unsigned short), 1, l_file);
				}
                break;
			
			case TRI_MAPPINGCOORS:
				fread (&l_qty, sizeof (unsigned short), 1, l_file);
				for (i=0; i<l_qty; i++)
				{
					fread (&data->mapcoord[i].u, sizeof (float), 1, l_file);
                    fread (&data->mapcoord[i].v, sizeof (float), 1, l_file);
				}
                break;

			/* Nos saltamos los desconocidos */
			default:
				 fseek(l_file, l_chunk_lenght-6, SEEK_CUR);
        } 
	}
	fclose (l_file);
	return 0;
}


/*! \fn void object_draw(t_obj_base object)
 *  \brief Dibuja un objeto mediante OpenGL en las coordenadas correspondientes
 *  \param object Objeto a dibujar
 *	\bug Falta el roll del objeto
*/

void object_draw(t_obj_base object)
{
	int l_index;
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	/*glBindTexture(GL_TEXTURE_2D, object.id_texture); ARREGLAR ESTO!, en teoría depende de la cara en la que estemos */

    glRotatef(object.rot.y, 0,0,1.0f);
    glRotatef(object.rot.x, 1.0f,0,0);
    //glRotatef(object.rot.z, sin(RAD(object.yaw)),cos(RAD(object.yaw)),-sin(RAD(object.pitch))); ARREGLAR!!!
    
    
    glTranslatef(object.pos.x,object.pos.y,object.pos.z);

    glBegin(GL_TRIANGLES);
	for (l_index=0;l_index<object.modelo.polygons_qty;l_index++)
    {
        /* Primer vértice */
        glTexCoord2f( object.modelo.mapcoord[ object.modelo.polygon[l_index].a ].u ,
                      object.modelo.mapcoord[ object.modelo.polygon[l_index].a ].v );
        glVertex3f( object.modelo.vertex[ object.modelo.polygon[l_index].a ].x *object.modelo.size,
                    object.modelo.vertex[ object.modelo.polygon[l_index].a ].y *object.modelo.size,
                    object.modelo.vertex[ object.modelo.polygon[l_index].a ].z *object.modelo.size);

        /* Segundo vértice */
        glTexCoord2f( object.modelo.mapcoord[ object.modelo.polygon[l_index].b ].u ,
                      object.modelo.mapcoord[ object.modelo.polygon[l_index].b ].v );
        glVertex3f( object.modelo.vertex[ object.modelo.polygon[l_index].b ].x *object.modelo.size,
                    object.modelo.vertex[ object.modelo.polygon[l_index].b ].y *object.modelo.size,
                    object.modelo.vertex[ object.modelo.polygon[l_index].b ].z *object.modelo.size);

        /* Tercer vértice */
        glTexCoord2f( object.modelo.mapcoord[ object.modelo.polygon[l_index].c ].u ,
                      object.modelo.mapcoord[ object.modelo.polygon[l_index].c ].v );
        glVertex3f( object.modelo.vertex[ object.modelo.polygon[l_index].c ].x *object.modelo.size,
                    object.modelo.vertex[ object.modelo.polygon[l_index].c ].y *object.modelo.size,
                    object.modelo.vertex[ object.modelo.polygon[l_index].c ].z *object.modelo.size);
    }
    glEnd();
}
