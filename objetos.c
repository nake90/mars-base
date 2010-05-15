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

/** \file objetos.c
 * \brief Control de objetos
 * Este archivo se encarga de todo lo relacionado con los objetos. (Carga, dibujo, control, ...)
 * \author Alfonso Arbona Gimeno
*/

#include "objetos.h"
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <IL/ilut.h>

/*
#ifdef WINDOWS
#include <io.h>
#endif
#ifdef LINUX*/
#include <dirent.h>
#include <sys/stat.h>

//#endif

static int get_material_index(t_model_ptr object, const char* name)
{
	int i;
	for (i=0; i<object->materials_qty; i++)
	{
		if (str_cmp(object->material[i].name,name)==0){return i;}
	}
	return -1;
}

void debug_get_3ds_chunk_name(unsigned short id, char* name)
{
	switch (id)
	{
		case (MAIN3DS): str_cpy(name,"MAIN3DS"); return;
		case (EDIT_MATERIAL): str_cpy(name,"EDIT_MATERIAL"); return;
		case (MAT_NAME): str_cpy(name,"MAT_NAME"); return;
		case (MAT_AMBIENT): str_cpy(name,"MAT_AMBIENT"); return;
		case (MAT_DIFFUSE): str_cpy(name,"MAT_DIFFUSE"); return;
		case (MAT_SPECULAR): str_cpy(name,"MAT_SPECULAR"); return;
		case (MAT_SHININESS): str_cpy(name,"MAT_SHININESS"); return;
		case (MAT_SHIN2PCT): str_cpy(name,"MAT_SHIN2PCT (DESCONOCIDO)"); return;
		case (MAT_SHIN3PCT): str_cpy(name,"MAT_SHIN3PCT (DESCONOCIDO)"); return;
		case (MAT_TRANSPARENCY): str_cpy(name,"MAT_TRANSPARENCY"); return;
		case (MAT_XPFALL): str_cpy(name,"MAT_XPFALL (DESCONOCIDO)"); return;
		case (MAT_REFBLUR): str_cpy(name,"MAT_REFBLUR (DESCONOCIDO)"); return;
		case (MAT_TEX1): str_cpy(name,"MAT_TEX1"); return;
		case (MAT_MASK1): str_cpy(name,"MAT_MASK1 (POSIBLE)"); return;
		case (MAT_SUB_RGB): str_cpy(name,"MAT_SUB_RGB"); return;
		case (MAT_SUB_PER): str_cpy(name,"MAT_SUB_PER"); return;
		case (MAT_MAPNAME): str_cpy(name,"MAT_MAPNAME"); return;
		case (EDIT3DS): str_cpy(name,"EDIT3DS"); return;
		case (EDIT_OBJECT): str_cpy(name,"EDIT_OBJECT"); return;
		case (OBJ_TRIMESH): str_cpy(name,"OBJ_TRIMESH"); return;
		case (TRI_VERTEXL): str_cpy(name,"TRI_VERTEXL"); return;
		case (TRI_VERTEXOPTIONS): str_cpy(name,"TRI_VERTEXOPTIONS (DESCONOCIDO)"); return;
		case (TRI_FACEL1): str_cpy(name,"TRI_FACEL1"); return;
		case (TRI_MATERIAL): str_cpy(name,"TRI_MATERIAL"); return;
		case (TRI_MAPPINGCOORS): str_cpy(name,"TRI_MAPPINGCOORS"); return;
		case (TRI_SMOOTH): str_cpy(name,"TRI_SMOOTH (POSIBLE)"); return;
		case (TRI_LOCAL): str_cpy(name,"TRI_LOCAL (POSIBLE)"); return;
		case (TRI_VISIBLE): str_cpy(name,"TRI_VISIBLE (POSIBLE)"); return;
		case (TRI_MAPPINGSTANDARD): str_cpy(name,"TRI_MAPPINGSTANDARD (DESCONOCIDO)"); return;
		default: sprintf(name,"DESCONOCIDO: %X",id); return;
	}
	sprintf(name,"DESCONOCIDO: %X",id);
}


/*! \fn int load_3DS (t_model *data, char *filename)
 *  \brief Carga los datos básicos de un objeto desde un archivo 3ds
 *  \param data Pointer al lugar donde se deben guardar los datos
 *  \param filename Ruta al archivo 3ds
 *  \return 0 -> Ok
 *  \return 1 -> Archivo no encontrado
 *  \return -1 -> Error interno
 *  \return -2 -> Error en mallocs (ver debug.log)
*/

int load_3DS (t_model *data, char *filename)
{
	if(objetos_debug > 0){debug_printf("\nDEBUG: - load_3DS - (debug level: %i) \"%s\"\n",objetos_debug, filename);}
	int i;
	int text_id;
	
	ILuint ImageName; //****
	
	FILE *l_file;
	
	unsigned short l_chunk_id;
	unsigned int l_chunk_lenght;

	unsigned short l_subchunk_id=0;
	
	unsigned char l_char;
	char string[80];
	char string2[256];
	char chunk_name[256];
	unsigned char l_rgb[3];
	unsigned short l_qty; /* Número de elementos en el chunk */

	unsigned short l_face_flags; /* Flag that stores some face information */

	if ((l_file=fopen (filename, "rb"))== NULL){debug_printf(TL_ERR_MODEL_NOT_FOUND,filename); return 1;}
	
	str_cpyl(string2,256,filename);
	str_ruta_get_filename(string2);
	str_ext_back(string2);
	
	str_cpyl(data->name,256,string2); // Guardamos el nombre del archivo
	if(objetos_debug > 0){debug_printf("INFO: - Objeto guardado como \"%s\"\n",data->name);}
	
	data->size=1.0f;
	data->materials_qty=0;
	data->draw_list=0;
	data->draw_lists=0;
	
	data->model_objetos_qty = 0; // Aún no hay ningún objeto cargado
	
	//#ifdef WINDOWS
	//while (ftell (l_file) < filelength(fileno(l_file))) /* Leemos todo el archivo */
	//#endif
	//#ifdef LINUX
	struct stat fileStatus;
	fstat( fileno(l_file), &fileStatus);
	while (ftell (l_file) < fileStatus.st_size) /* Leemos todo el archivo */
	//#endif
	{
		fread (&l_chunk_id, 2, 1, l_file); /* HEADER (tipo de chunk) */
		fread (&l_chunk_lenght, 4, 1, l_file); /* Length */

		if(objetos_debug > 1)
		{
			debug_get_3ds_chunk_name(l_chunk_id,chunk_name);
			debug_printf("DEBUG:   CHUNK: %X (%s), LENGTH: %i bytes\n",l_chunk_id,chunk_name,l_chunk_lenght);
		}
		switch (l_chunk_id)
        {
			case MAIN3DS: /* Lenght: 0 + sub chunks */
			if(objetos_debug > 1){debug_printf("DEBUG:    MAIN3DS\n");}
			break;    
			
			case EDIT3DS: /* Lenght: 0 + sub chunks */
			if(objetos_debug > 1){debug_printf("DEBUG:    EDIT3DS\n");}
			break;
			
			case EDIT_OBJECT:
			if(objetos_debug > 1){debug_printf("DEBUG:    EDIT_OBJECT\n");}
				i=0;
				data->model_objetos_qty++;
				/* MALLOC'S */
					if(data->model_objetos_qty<=1)
					{
						data->model_objetos_qty=1;
						// Todo a NULL
						data->obj_name=NULL;
						//data->obj_name[0]=NULL; No aquí, primero hacemos el realloc grande y después inicializamos
						data->vertices_qty=NULL;
						data->polygons_qty=NULL;
						data->vertex=NULL;
						data->polygon=NULL;
						data->mapcoord=NULL;
					}
					// obj_name
					data->obj_name=realloc(data->obj_name,sizeof(char*)*data->model_objetos_qty);
					if(data->obj_name==NULL){debug_printf(TL_ERR_REALLOC, "de obj_name"); return -2;}
					data->obj_name[data->model_objetos_qty-1]=NULL;
					data->obj_name[data->model_objetos_qty-1]=realloc(data->obj_name[data->model_objetos_qty-1],sizeof(char)*20);
					if(data->obj_name[data->model_objetos_qty-1]==NULL){debug_printf(TL_ERR_REALLOC, "de obj_name[]"); return -2;}
					// vertices_qty
					data->vertices_qty = realloc(data->vertices_qty, sizeof(int)*data->model_objetos_qty);
					if(data->vertices_qty==NULL){debug_printf(TL_ERR_REALLOC, "de vertices_qty"); return -2;}
					// polygons_qty
					data->polygons_qty = realloc(data->polygons_qty, sizeof(int)*data->model_objetos_qty);
					if(data->polygons_qty==NULL){debug_printf(TL_ERR_REALLOC, "de polygons_qty"); return -2;}
					// vertex
					data->vertex=realloc(data->vertex,sizeof(VECTOR*)*data->model_objetos_qty);
					if(data->vertex==NULL){debug_printf(TL_ERR_REALLOC, "de vertex"); return -2;}
					data->vertex[data->model_objetos_qty-1]=NULL;
					data->vertex[data->model_objetos_qty-1]=realloc(data->vertex[data->model_objetos_qty-1],sizeof(VECTOR)*MAX_VERTICES);
					if(data->vertex[data->model_objetos_qty-1]==NULL){debug_printf(TL_ERR_REALLOC, "de vertex[]"); return -2;}
					// polygon
					data->polygon=realloc(data->polygon,sizeof(t_polygon*)*data->model_objetos_qty);
					if(data->polygon==NULL){debug_printf(TL_ERR_REALLOC, "de polygon"); return -2;}
					data->polygon[data->model_objetos_qty-1]=NULL;
					data->polygon[data->model_objetos_qty-1]=realloc(data->polygon[data->model_objetos_qty-1],sizeof(t_polygon)*MAX_POLYGONS);
					if(data->polygon[data->model_objetos_qty-1]==NULL){debug_printf(TL_ERR_REALLOC, "de polygon[]"); return -2;}
					// mapcoord
					data->mapcoord=realloc(data->mapcoord,sizeof(t_mapcoord*)*data->model_objetos_qty);
					if(data->mapcoord==NULL){debug_printf(TL_ERR_REALLOC, "de mapcoord"); return -2;}
					data->mapcoord[data->model_objetos_qty-1]=NULL;
					data->mapcoord[data->model_objetos_qty-1]=realloc(data->mapcoord[data->model_objetos_qty-1],sizeof(t_mapcoord)*MAX_VERTICES);
					if(data->mapcoord[data->model_objetos_qty-1]==NULL){debug_printf(TL_ERR_REALLOC, "de mapcoord[]"); return -2;}
				/* END MALLOC'S */
				do
				{
					fread (&l_char, 1, 1, l_file);
                    data->obj_name[data->model_objetos_qty-1][i]=l_char;
					i++;
                }while(l_char != '\0' && i<20);
			if(objetos_debug > 1){debug_printf("DEBUG:     name: \"%s\"\n",data->name);}
			break;
			
			case OBJ_TRIMESH: /* Lenght: 0 + sub chunks */
			if(objetos_debug > 1){debug_printf("DEBUG:    OBJ_TRIMESH\n");}
			break;
			
			case TRI_VERTEXL:
			if(objetos_debug > 1){debug_printf("DEBUG:    TRI_VERTEXL\n");}
				fread (&l_qty, sizeof (unsigned short), 1, l_file);
                data->vertices_qty[data->model_objetos_qty-1] = l_qty;
                for (i=0; i<l_qty; i++)
                {
					fread (&data->vertex[data->model_objetos_qty-1][i].x, sizeof(float), 1, l_file);
                    fread (&data->vertex[data->model_objetos_qty-1][i].y, sizeof(float), 1, l_file);
					fread (&data->vertex[data->model_objetos_qty-1][i].z, sizeof(float), 1, l_file);
				}
				break;
			
			case TRI_FACEL1:
			if(objetos_debug > 1){debug_printf("DEBUG:    TRI_FACEL1\n");}
				fread (&l_qty, sizeof (unsigned short), 1, l_file);
                data->polygons_qty[data->model_objetos_qty-1] = l_qty;
                for (i=0; i<l_qty; i++)
                {
					fread (&data->polygon[data->model_objetos_qty-1][i].a, sizeof (unsigned short), 1, l_file);
					fread (&data->polygon[data->model_objetos_qty-1][i].b, sizeof (unsigned short), 1, l_file);
					fread (&data->polygon[data->model_objetos_qty-1][i].c, sizeof (unsigned short), 1, l_file);
					fread (&l_face_flags, sizeof (unsigned short), 1, l_file);
				}
                break;
			
			case TRI_MAPPINGCOORS:
			if(objetos_debug > 1){debug_printf("DEBUG:    TRI_MAPPINGCOORDS\n");}
				fread (&l_qty, sizeof (unsigned short), 1, l_file);
				for (i=0; i<l_qty; i++)
				{
					fread (&data->mapcoord[data->model_objetos_qty-1][i].u, sizeof (float), 1, l_file);
                    fread (&data->mapcoord[data->model_objetos_qty-1][i].v, sizeof (float), 1, l_file);
				}
                break;
                
			case TRI_MATERIAL:
				i=0;
				do
				{
					fread (&l_char, 1, 1, l_file);
                    string[i]=(char)l_char;
					i++;
                }while(l_char != '\0' && i<80);
                text_id=get_material_index(data,string);
                if(objetos_debug > 1){debug_printf("DEBUG:     Busqueda de nombre de textura retorna %i\n",text_id);}
                if (text_id==-1){debug_printf("ERROR: Busqueda de nombre de textura \"%s\" fallido!\n",string);fclose (l_file);return -1;}
                fread (&l_qty, sizeof (unsigned short), 1, l_file);
                for (i=0; i<l_qty; i++)
				{
					fread (&l_face_flags, sizeof (unsigned short), 1, l_file);
					data->polygon[data->model_objetos_qty-1][l_face_flags].texture=data->material[text_id].texture[0];
					if(objetos_debug > 2){debug_printf("DEBUG:     Face %i asignado al material %i, textura: %i\n",l_face_flags,text_id,data->material[text_id].texture[0]);}
				}
                break;
                
			case EDIT_MATERIAL: /* Lenght: 0 + sub chunks */
			if(objetos_debug > 1){debug_printf("DEBUG:    EDIT_MATERIAL\n");}
				break;
			
			case MAT_NAME:
			if(objetos_debug > 1){debug_printf("DEBUG:    MAT_NAME\n");}
				data->materials_qty++;
				i=0;
				do
				{
					fread (&l_char, 1, 1, l_file);
                    data->material[data->materials_qty-1].name[i]=l_char;
					i++;
                }while(l_char != '\0' && i<20);
				if(objetos_debug > 1){debug_printf("DEBUG:     name: \"%s\"\n",data->material[data->materials_qty-1].name);}
                break;
			
			case MAT_AMBIENT:
			case MAT_DIFFUSE:
			case MAT_SPECULAR:
			case MAT_SHININESS:
			case MAT_TRANSPARENCY:
			if(objetos_debug > 1){debug_printf("DEBUG:    l_subchunk_id=%X\n",l_chunk_id);}
				l_subchunk_id=l_chunk_id;
                break;
                
                
			case MAT_SUB_RGB:
			if(objetos_debug > 1){debug_printf("DEBUG:    MAT_SUB_RGB (l_subchunk_id=%X)\n",l_subchunk_id);}
				fread (l_rgb, 1, 3, l_file);
				if(objetos_debug > 2){debug_printf("DEBUG:     RGB(%hi,%hi,%hi)\n",l_rgb[0],l_rgb[1],l_rgb[2]);}
				switch (l_subchunk_id)
				{
					case MAT_AMBIENT:
						data->material[data->materials_qty-1].ambient[0]=l_rgb[0];
						data->material[data->materials_qty-1].ambient[1]=l_rgb[1];
						data->material[data->materials_qty-1].ambient[2]=l_rgb[2];
						data->material[data->materials_qty-1].ambient[3]=1.0f;
						break;
					case MAT_DIFFUSE:
						data->material[data->materials_qty-1].diffuse[0]=l_rgb[0];
						data->material[data->materials_qty-1].diffuse[1]=l_rgb[1];
						data->material[data->materials_qty-1].diffuse[2]=l_rgb[2];
						data->material[data->materials_qty-1].diffuse[3]=1.0f;
						break;
					case MAT_SPECULAR:
						data->material[data->materials_qty-1].specular[0]=l_rgb[0];
						data->material[data->materials_qty-1].specular[1]=l_rgb[1];
						data->material[data->materials_qty-1].specular[2]=l_rgb[2];
						data->material[data->materials_qty-1].specular[3]=1.0f;
						break;
				}
                break;
                
			case MAT_SUB_PER:
			if(objetos_debug > 1){debug_printf("DEBUG:    MAT_SUB_PER (l_subchunk_id=%X)\n",l_subchunk_id);}
				fread (&l_qty, sizeof (unsigned short), 1, l_file);
				switch (l_subchunk_id)
				{
					case MAT_SHININESS:
						data->material[data->materials_qty-1].shininess[0]=l_qty;
						break;
					case MAT_TRANSPARENCY:
						data->material[data->materials_qty-1].transparency[0]=l_qty;
						break;
				}
                break;
			
			case MAT_TEX1:
			if(objetos_debug > 1){debug_printf("DEBUG:    MAT_TEX1\n");}
				break;
			
			case MAT_MAPNAME:
			if(objetos_debug > 1){debug_printf("DEBUG:    MAT_MAPNAME\n");}
				i=0;
				do
				{
					fread (&l_char, 1, 1, l_file);
                    string[i]=(char)l_char;
					i++;
                }while(l_char != '\0' && i<80);
                if(i==80){debug_printf("ALERTA: Nombre de textura cortado. (No debería de haber pasado nunca, probablemente sea por un archivo corrupto)\n",string);}
                //str_append(string,".tga");
                str_cpy(string2,app_path);
                str_append(string2,"materials\\");
                str_append(string2,string);
                /* Cargamos la textura */
				//data->material[data->materials_qty-1].texture[0]=ilutGLLoadImage(string2);
				ilGenImages(1, &ImageName); //****
				ilBindImage(ImageName); //****
				
				//if(!data->material[data->materials_qty-1].texture[0])
				if(!ilLoadImage(string2))
				{
					str_append(string2,".tga"); // No se ha encontrado? Veamos si le falta el .tga
					//data->material[data->materials_qty-1].texture[0]=ilutGLLoadImage(string2);
					//if(!data->material[data->materials_qty-1].texture[0])
					if(!ilLoadImage(string2))
					{
						str_ext_back(string2);
						str_append(string2,".jpg");
						//data->material[data->materials_qty-1].texture[0]=ilutGLLoadImage(string2);
						//if(!data->material[data->materials_qty-1].texture[0])
						if(!ilLoadImage(string2))
						{
							data->material[data->materials_qty-1].texture[0]=null_texture;
							debug_printf(lista_texto[TEXT_LIST_R_WRN + 3],string);
							break;
						}
					}
				}
				data->material[data->materials_qty-1].texture[0] = ilutGLBindTexImage(); //****
				
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				ilutGLBuildMipmaps();
				
				ilDeleteImages(1, &ImageName); //****
				
				if(objetos_debug > 1){debug_printf("DEBUG:     Material %i, textura cargada como %i\n",data->materials_qty-1,data->material[data->materials_qty-1].texture[0]);}
				break;
			
			
			/* Nos saltamos los desconocidos */
			default:
			if(objetos_debug > 1){debug_printf("DEBUG:    SKIPPED!\n");}
				 fseek(l_file, l_chunk_lenght-6, SEEK_CUR);
        } 
	}
	if(objetos_debug > 0){debug_printf("\n");}
	fclose (l_file);
	return 0;
}


/*! \fn void model_predraw(t_model *modelo)
 *  \brief Dibuja un modelo mediante OpenGL en la lista correspondiente
 *  \param modelo Modelo a dibujar
*/

void model_predraw(t_model *modelo)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	int l_index;
	int objeto_actual;
	
	glDisable(GL_LIGHTING);
	
	modelo->draw_list=glGenLists(1);
	modelo->draw_lists=1;
	
	glColor3f(1.0f,1.0f,1.0f); // Si lo pongo antes de GL_COMPILE puedo cambiar el color y la transparencia durante el juego!!
	glNewList(modelo->draw_list,GL_COMPILE);
	
    glDisable(GL_CULL_FACE);
    //glEnable(GL_CULL_FACE); *** CAMBIAR A ESTO TAN PRONTO COMO LOS OBJETOS LO PERMITAN
	glEnable(GL_TEXTURE_2D);
	
	for(objeto_actual=0; objeto_actual < modelo->model_objetos_qty; objeto_actual++)
	{
		
		for (l_index=0;l_index < modelo->polygons_qty[objeto_actual];l_index++)
	    {
		    if(modelo->polygon[objeto_actual][l_index].texture!=0)
			{
				glBindTexture(GL_TEXTURE_2D, modelo->polygon[objeto_actual][l_index].texture);
			}
			else
			{
			    glBindTexture(GL_TEXTURE_2D, null_texture);
			}
			glBegin(GL_TRIANGLES);
				/* Primer vértice */
		        glTexCoord2f( modelo->mapcoord[objeto_actual][ modelo->polygon[objeto_actual][l_index].a ].u ,
		                      modelo->mapcoord[objeto_actual][ modelo->polygon[objeto_actual][l_index].a ].v );
		        glVertex3f( modelo->vertex[objeto_actual][ modelo->polygon[objeto_actual][l_index].a ].x *modelo->size,
		                    modelo->vertex[objeto_actual][ modelo->polygon[objeto_actual][l_index].a ].y *modelo->size,
		                    modelo->vertex[objeto_actual][ modelo->polygon[objeto_actual][l_index].a ].z *modelo->size);
		
		        /* Segundo vértice */
		        glTexCoord2f( modelo->mapcoord[objeto_actual][ modelo->polygon[objeto_actual][l_index].b ].u ,
		                      modelo->mapcoord[objeto_actual][ modelo->polygon[objeto_actual][l_index].b ].v );
		        glVertex3f( modelo->vertex[objeto_actual][ modelo->polygon[objeto_actual][l_index].b ].x *modelo->size,
		                    modelo->vertex[objeto_actual][ modelo->polygon[objeto_actual][l_index].b ].y *modelo->size,
		                    modelo->vertex[objeto_actual][ modelo->polygon[objeto_actual][l_index].b ].z *modelo->size);
		
		        /* Tercer vértice */
		        glTexCoord2f( modelo->mapcoord[objeto_actual][ modelo->polygon[objeto_actual][l_index].c ].u ,
		                      modelo->mapcoord[objeto_actual][ modelo->polygon[objeto_actual][l_index].c ].v );
		        glVertex3f( modelo->vertex[objeto_actual][ modelo->polygon[objeto_actual][l_index].c ].x *modelo->size,
		                    modelo->vertex[objeto_actual][ modelo->polygon[objeto_actual][l_index].c ].y *modelo->size,
		                    modelo->vertex[objeto_actual][ modelo->polygon[objeto_actual][l_index].c ].z *modelo->size);
		    glEnd();
		}
	}
	glEnable(GL_CULL_FACE);
	glEndList();
	
	l_index=glGetError();
	if (l_index){debug_printf("ERROR: OpenGL ha retornado %i al llamar a model_predraw()\n",l_index);}
	
	glPopMatrix();
}


/*! \fn void object_draw_l(t_obj_base object)
 *  \brief Dibuja un objeto mediante listas de OpenGL en las coordenadas correspondientes
 *  \param object Objeto a dibujar
 *	\bug Falta el roll del objeto
 *	\sa model_predraw
*/

void object_draw_l(t_obj_base_ptr object)
{
	//int l_index;
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glEnable(GL_LIGHTING);
    
    glTranslatef(object->pos.x,object->pos.y,object->pos.z);
    
    glRotatef(object->rot.z, 0,1.0f,0);
    glRotatef(object->rot.y, 0,0,1.0f);
    glRotatef(object->rot.x, 1.0f,0,0);
    
    glCallList(object->modelo->draw_list);
	glPopMatrix();
}

void object_draw_nodes(t_obj_base_ptr object)
{
	//int l_index;
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glDisable(GL_LIGHTING);
	
	glTranslatef(object->pos.x,object->pos.y,object->pos.z);
	
    glRotatef(object->rot.z, 0,1.0f,0);
    glRotatef(object->rot.y, 0,0,1.0f);
    glRotatef(object->rot.x, 1.0f,0,0);
	
	glLineWidth(2.0f);
	glPointSize(6.0f);
	int i;
	for(i=0;i<object->conx_qty;i++)
	{
		glColor4f(0.0f,1.0f,0.0f,1.0f);
		glBegin(GL_POINTS);
			glVertex3f(	object->conx_coord[i].x,
						object->conx_coord[i].y,
						object->conx_coord[i].z+1);
		glEnd();
		if(object->conx_id[i]==-1)
		{
			glColor4f(0.0f,0.0f,1.0f,0.7f); // Desconectado
		}
		else
		{
			glColor4f(0.0f,1.0f,0.0f,0.7f); // Conectado
		}
		glBegin(GL_LINES);
			glVertex3f(	object->conx_coord[i].x,
						object->conx_coord[i].y,
						object->conx_coord[i].z+1);
			glVertex3f(	object->conx_coord[i].x + object->conx_norm[i].x,
						object->conx_coord[i].y + object->conx_norm[i].y,
						object->conx_coord[i].z + object->conx_norm[i].z +1);
		glEnd();
	}
	
	glPopMatrix();
}


void object_draw_selected(t_obj_base_ptr object)
{
	//int l_index;
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glDisable(GL_LIGHTING);
	
	glTranslatef(object->pos.x,object->pos.y,object->pos.z);
	
    glRotatef(object->rot.z, 0,1.0f,0);
    glRotatef(object->rot.y, 0,0,1.0f);
    glRotatef(object->rot.x, 1.0f,0,0);
	
	glLineWidth(2.8f);
	glColor4f(0.0,0.0,1.0,0.8f);
	glBegin(GL_LINE_LOOP);
		glVertex3f(object->sq_l -0.5, object->sq_t +0.5, +0.01);
		glVertex3f(object->sq_l -0.5, object->sq_b -0.5, +0.01);
		glVertex3f(object->sq_r +0.5, object->sq_b -0.5, +0.01);
		glVertex3f(object->sq_r +0.5, object->sq_t +0.5, +0.01);
	glEnd();
	
	glPopMatrix();
}

/*! \fn int lista_base_crear_elemento(t_obj_base_ptr *lista, int* contador, char *ruta)
 *  \brief Carga un objeto tipo base y lo añade a la lista.
 *  \param lista La lista a la cual se debe añadir el elemento
 *  \param contador Contador de la lista
 *  \param ruta Ruta al archivo 3ds
 *	\return 0 si todo fué bien.
 *	\return 1 Archivo no encontrado.
 *	\return -1 Error interno del archivo 3ds.
 *	\return -2 Error en el malloc del objeto base.
 *	\return -3 Error en el malloc del modelo.
*/

int lista_base_crear_elemento(int id_modelo)
{
	if (id_modelo>=lista_modelos || id_modelo<0){debug_printf(TL_ERR_LIST_CREATE_MODEL_ID); return -3;}
	
	if(lista_objeto_base!=NULL)
	{
		lista_objeto_base=realloc(lista_objeto_base, sizeof(t_obj_base*)*(lista_objetos_base+1));
		if (lista_objeto_base==NULL){debug_printf(TL_ERR_REALLOC, "de objetos"); lista_objetos_base=-1; return -1;}
	}
	else
	{
		lista_objetos_base=0;
		lista_objeto_base=malloc(sizeof(t_obj_base*));
		if (lista_objeto_base==NULL){debug_printf(TL_ERR_MALLOC, "de la lista de objetos"); lista_objetos_base=-1; return -1;}
	}
	
	lista_objeto_base[lista_objetos_base]=malloc(sizeof(t_obj_base));
	if (lista_objeto_base[lista_objetos_base]==NULL){debug_printf(TL_ERR_MALLOC_ID, "del objeto", lista_objetos_base); return -2;}
	
	//memset(lista_objeto_base[lista_objetos_base], 0, sizeof(lista_objeto_base[lista_objetos_base]));
	
	lista_objeto_base[lista_objetos_base]->modelo = lista_modelo[id_modelo];
	
	/* Inicialización */
	sprintf(lista_objeto_base[lista_objetos_base]->name,"%s %i",lista_modelo[id_modelo]->name,lista_objetos_base); // Como nombre predeterminado el nombre del modelo y su ID
	obj_ptr_setpos(lista_objeto_base[lista_objetos_base],0,0,0);
	lista_objeto_base[lista_objetos_base]->selec = 0;
	lista_objeto_base[lista_objetos_base]->rot.x=0;
	lista_objeto_base[lista_objetos_base]->rot.y=0;
	lista_objeto_base[lista_objetos_base]->rot.z=0;
	lista_objeto_base[lista_objetos_base]->sq_l=0;
	lista_objeto_base[lista_objetos_base]->sq_r=0;
	lista_objeto_base[lista_objetos_base]->sq_t=0;
	lista_objeto_base[lista_objetos_base]->sq_b=0;
	lista_objeto_base[lista_objetos_base]->conx_qty=0;
    int con;
    VECTOR vec_zero={0,0,0};
    for(con=0;con<MAX_CONX;con++)
    {
		lista_objeto_base[lista_objetos_base]->conx_coord[con] = vec_zero;
		lista_objeto_base[lista_objetos_base]->conx_norm[con] = vec_zero;
		lista_objeto_base[lista_objetos_base]->conx_id[con] = -1;
		lista_objeto_base[lista_objetos_base]->conx_node_id[con] = -1;
		lista_objeto_base[lista_objetos_base]->conx_size[con] = 1.0f;
	}
    //lista_objeto_base[lista_objetos_base]->volumen = 0;
    lista_objeto_base[lista_objetos_base]->reparar = 0;
	//lista_objeto_base[lista_objetos_base]->temperatura = 0;
	lista_objeto_base[lista_objetos_base]->node_data.volumen = 0;
	lista_objeto_base[lista_objetos_base]->node_data.temperatura = 0;
	
	lista_objeto_base[lista_objetos_base]->node_data.gases.CO2 = 0;
	lista_objeto_base[lista_objetos_base]->node_data.gases.N2 = 0;
	lista_objeto_base[lista_objetos_base]->node_data.gases.Ar = 0;
	lista_objeto_base[lista_objetos_base]->node_data.gases.O2 = 0;
	lista_objeto_base[lista_objetos_base]->node_data.gases.CO = 0;
	lista_objeto_base[lista_objetos_base]->node_data.gases.H2O = 0;
	    
	//object_predraw(lista_objeto_base[lista_objetos_base]);
	
	lista_objetos_base++;
	
    return 0;
}

int lista_modelo_get_id(const char* nombre_modelo)
{
	int i;
	for(i=0; i<lista_modelos; i++)
	{
		if(str_cmp(nombre_modelo, lista_modelo[i]->name)==0){return i;}
	}
	return -1;
}


/*! \fn int lista_cargar_modelo_dir(const char *dir)
 *  \brief Carga todos los modelos tipo 3ds en la carpeta indicada por dir y sus subcarpetas
 *  \param *dir Ruta a la carpeta donde se debe buscar y cargar los archivos.
*/
#ifdef WINDOWS
int lista_cargar_modelo_dir(const char *dir)
{
	struct _finddata_t datos; // Datos del archivo o carpeta encontrados
	char dir_buffer[1024];
	char buffer[1024];
	long handle;
	
	str_cpyl(dir_buffer,1023,dir);
	str_append(dir_buffer,"/*"); // Indicamos que queremos buscar en toda la carpeta
	
	handle = _findfirst (dir_buffer, &datos);
	if(handle==-1){debug_printf(TL_ERR_MODEL_DIR,dir_buffer);return -1;}
	do
	{
		if( !(datos.attrib & _A_SUBDIR) && str_ext_cmp(datos.name,"3ds")==1) // Si es de tipo 3ds y no es un directorio
		{
			str_cpyl(buffer,1023,dir);
			str_append(buffer,"/");
			str_append(buffer,datos.name);
			debug_printf(lista_texto[TEXT_LIST_R_NFO + 3],datos.name);
			lista_cargar_modelo(buffer);
		}
		else if(datos.attrib & _A_SUBDIR && str_cmp(datos.name,".")!=0 && str_cmp(datos.name,"..")!=0) // Si es una carpeta llamamos a esta misma función y au!
		{
			//debug_printf("Buscando en carpeta \"%s\"\n",datos.name);
			str_cpyl(dir_buffer,1023,dir);
			str_append(dir_buffer,"/");
			str_append(dir_buffer,datos.name);
			lista_cargar_modelo_dir(dir_buffer); // Buscamos en esa carpeta
		}
		
	}while(_findnext(handle, &datos)!=-1);
	
	_findclose(handle);
	return 0;
}
#endif
#ifdef LINUX
int lista_cargar_modelo_dir(const char *dir)
{
	struct dirent *datos; // Datos del archivo o carpeta encontrados
	char dir_buffer[1024];
	char buffer[1024];
	DIR *handle;
	
	str_cpyl(dir_buffer,1023,dir);
	str_append(dir_buffer,"/*"); // Indicamos que queremos buscar en toda la carpeta
	
	handle = opendir(dir_buffer);
	if(handle==NULL){debug_printf(TL_ERR_MODEL_DIR,dir_buffer);return -1;}
	while(datos=readdir(handle)!=NULL);
	{
		if( !(datos->d_type == DT_DIR) && str_ext_cmp(datos->d_name,"3ds")==1) // Si es de tipo 3ds y no es un directorio
		{
			str_cpyl(buffer,1023,dir);
			str_append(buffer,"/");
			str_append(buffer,datos->d_name);
			debug_printf(lista_texto[TEXT_LIST_R_NFO + 3],datos->d_name);
			lista_cargar_modelo(buffer);
		}
		else if(datos->d_type == DT_DIR && str_cmp(datos->d_name,".")!=0 && str_cmp(datos->d_name,"..")!=0) // Si es una carpeta llamamos a esta misma función y au!
		{
			//debug_printf("Buscando en carpeta \"%s\"\n",datos.name);
			str_cpyl(dir_buffer,1023,dir);
			str_append(dir_buffer,"/");
			str_append(dir_buffer,datos->d_name);
			lista_cargar_modelo_dir(dir_buffer); // Buscamos en esa carpeta
		}
		
	}

	
	closedir(handle);
	
	return 0;
}
#endif

int lista_cargar_modelo(char *ruta)
{
	if(lista_modelo!=NULL)
	{
		lista_modelo=realloc(lista_modelo, sizeof(t_model*)*(lista_modelos+1));
		if (lista_modelo==NULL){debug_printf(TL_ERR_REALLOC, "de la lista de modelos"); lista_modelos=-1; return -1;}
	}
	else
	{
		lista_modelos=0;
		lista_modelo=malloc(sizeof(t_model*));
		if (lista_modelo==NULL){debug_printf(TL_ERR_MALLOC, "de la lista de modelos"); lista_modelos=-1; return -1;}
	}
	
	
	lista_modelo[lista_modelos]=malloc(sizeof(t_model));
	if(lista_modelo[lista_modelos]==NULL){debug_printf(TL_ERR_MALLOC_ID, "del modelo", lista_modelos); return -2;}
	
	int i;
	i=load_3DS(lista_modelo[lista_modelos], ruta);
	
	if (i!=0){debug_printf(TL_ERR_MODEL_LOAD,ruta,i); free(lista_modelo[lista_modelos]); return -i-10;}
	
	// Cargamos el icono (si existe)
	char buffer[1024];
	str_cpyl(buffer,1023,ruta);
	str_ext_back(buffer);
	str_append(buffer,"_spawn.tga");
	
	ILuint ImageName;
	ilGenImages(1, &ImageName);
	ilBindImage(ImageName);
	
	//lista_modelo[lista_modelos]->icono = ilutGLLoadImage(buffer);
	//if(!lista_modelo[lista_modelos]->icono)
	if(!ilLoadImage(buffer))
	{
		debug_printf(lista_texto[TEXT_LIST_R_WRN + 4],buffer);
		lista_modelo[lista_modelos]->icono = icono_no_icon;
	}
	else
	{
		lista_modelo[lista_modelos]->icono = ilutGLBindTexImage();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		ilutGLBuildMipmaps();
		ilDeleteImages(1,&lista_modelo[lista_modelos]->icono);
	}
	
	model_predraw(lista_modelo[lista_modelos]);
	
	lista_modelos++;
	
    return 0;
}


/*! \fn void lista_base_limpiar(t_obj_base_ptr *lista, int* contador)
 *  \brief Limpia de la memoria todos los elementos de la lista y borra la lista
 *  \warning No intentar dibujar la lista después de usar esta función.
 *  \param lista La lista que contiene los elementos que se deben borrar
 *  \param contador Contador de la lista
*/

void lista_base_limpiar(void)
{
	debug_printf(lista_texto[TEXT_LIST_R_NFO + 4],lista_objetos_base);
	lista_objetos_base--;
    while(lista_objetos_base>=0)
	{
		free(lista_objeto_base[lista_objetos_base]);
		lista_objetos_base--;
	}
	free(lista_objeto_base);
	lista_objetos_base=0;
	lista_objeto_base=NULL;
}

void model_unload(t_model *model)
{
	int i;
	// OPENGL LISTS
	glDeleteLists(model->draw_list,model->draw_lists);
	model->draw_lists=0;
	
	// MATRICES
	for (i=0; i<model->model_objetos_qty; i++)
	{
		free(model->obj_name[i]);
		free(model->vertex[i]);
		free(model->polygon[i]);
		free(model->mapcoord[i]);
	}
	
	free(model->obj_name);
	free(model->vertex);
	free(model->polygon);
	free(model->mapcoord);
	free(model->vertices_qty);
	free(model->polygons_qty);
	
	// MATERIALES
	ilBindImage(0);
	for (i=0; i<model->materials_qty; i++)
	{
		glDeleteTextures(1, &model->material[i].texture[0]);
		model->material[i].texture[0]=0;
	}
}

void lista_modelos_limpiar(void)
{
	debug_printf(lista_texto[TEXT_LIST_R_NFO + 5],lista_modelos);
	lista_modelos--;
    while(lista_modelos>=0)
	{
		model_unload(lista_modelo[lista_modelos]);
		free(lista_modelo[lista_modelos]);
		lista_modelos--;
	}
	free(lista_modelo);
	lista_modelos=0;
	lista_modelo=NULL;
}
