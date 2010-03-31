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

static int get_material_index(t_model_ptr object, const char* name)
{
	int i;
	for (i=0; i<object->materials_qty; i++)
	{
		if (str_cmp(object->material[i].name,name)==0){return i;}
	}
	return -1;
}

/*! \fn int load_3DS (t_model *data, char *filename)
 *  \brief Carga los datos b�sicos de un objeto desde un archivo 3ds
 *  \param data Pointer al lugar donde se deben guardar los datos
 *  \param filename Ruta al archivo 3ds
 *  \return 0 -> Ok; 1 -> Archivo no encontrado; -1 -> Error interno
*/

int load_3DS (t_model *data, char *filename)
{
	if(objetos_debug){debug_printf("DEBUG: - load_3DS - (debug level: %i) \"%s\"\n",objetos_debug, filename);}
	int i;
	int text_id;
	
	FILE *l_file;
	
	unsigned short l_chunk_id;
	unsigned int l_chunk_lenght;

	unsigned short l_subchunk_id=0;
	
	unsigned char l_char;
	char string[80];
	char string2[256];
	unsigned char l_rgb[3];
	unsigned short l_qty; /* N�mero de elementos en el chunk */

	unsigned short l_face_flags; /* Flag that stores some face information */

	if ((l_file=fopen (filename, "rb"))== NULL){debug_printf("ERROR: Modelo \"%s\" no encontrado.\n",filename); return 1;}
	
	data->size=1.0f;
	data->materials_qty=0;
	data->draw_list=0;
	data->draw_lists=0;
	
	while (ftell (l_file) < filelength (fileno (l_file))) /* Leemos todo el archivo */
	{
		fread (&l_chunk_id, 2, 1, l_file); /* HEADER (tipo de chunk) */
		fread (&l_chunk_lenght, 4, 1, l_file); /* Length */

		if(objetos_debug){debug_printf("DEBUG:   CHUNK: %X, LENGTH: %i bytes\n",l_chunk_id,l_chunk_lenght);}
		switch (l_chunk_id)
        {
			case MAIN3DS: /* Lenght: 0 + sub chunks */
			if(objetos_debug){debug_printf("DEBUG:    MAIN3DS\n");}
			break;    
			
			case EDIT3DS: /* Lenght: 0 + sub chunks */
			if(objetos_debug){debug_printf("DEBUG:    EDIT3DS\n");}
			break;
			
			case EDIT_OBJECT:
			if(objetos_debug){debug_printf("DEBUG:    EDIT_OBJECT\n");}
				i=0;
				do
				{
					fread (&l_char, 1, 1, l_file);
                    data->name[i]=l_char;
					i++;
                }while(l_char != '\0' && i<20);
			if(objetos_debug){debug_printf("DEBUG:     name: \"%s\"\n",data->name);}
			break;
			
			case OBJ_TRIMESH: /* Lenght: 0 + sub chunks */
			if(objetos_debug){debug_printf("DEBUG:    OBJ_TRIMESH\n");}
			break;
			
			case TRI_VERTEXL:
			if(objetos_debug){debug_printf("DEBUG:    TRI_VERTEXL\n");}
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
			if(objetos_debug){debug_printf("DEBUG:    TRI_FACEL1\n");}
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
			if(objetos_debug){debug_printf("DEBUG:    TRI_MAPPINGCOORDS\n");}
				fread (&l_qty, sizeof (unsigned short), 1, l_file);
				for (i=0; i<l_qty; i++)
				{
					fread (&data->mapcoord[i].u, sizeof (float), 1, l_file);
                    fread (&data->mapcoord[i].v, sizeof (float), 1, l_file);
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
                if(objetos_debug>=2){debug_printf("DEBUG:     Busqueda de nombre de textura retorna %i\n",text_id);}
                if (text_id==-1){debug_printf("ERROR: Busqueda de nombre de textura \"%s\" fallido!\n",string);fclose (l_file);return -1;}
                fread (&l_qty, sizeof (unsigned short), 1, l_file);
                for (i=0; i<l_qty; i++)
				{
					fread (&l_face_flags, sizeof (unsigned short), 1, l_file);
					data->polygon[l_face_flags].texture=data->material[text_id].texture[0];
					if(objetos_debug>=3){debug_printf("DEBUG:     Face %i asignado al material %i, textura: %i\n",l_face_flags,text_id,data->material[text_id].texture[0]);}
				}
                break;
                
			case EDIT_MATERIAL: /* Lenght: 0 + sub chunks */
			if(objetos_debug){debug_printf("DEBUG:    EDIT_MATERIAL\n");}
				break;
			
			case MAT_NAME:
			if(objetos_debug){debug_printf("DEBUG:    MAT_NAME\n");}
				data->materials_qty++;
				i=0;
				do
				{
					fread (&l_char, 1, 1, l_file);
                    data->material[data->materials_qty-1].name[i]=l_char;
					i++;
                }while(l_char != '\0' && i<20);
				if(objetos_debug){debug_printf("DEBUG:     name: \"%s\"\n",data->material[data->materials_qty-1].name);}
                break;
			
			case MAT_AMBIENT:
			case MAT_DIFFUSE:
			case MAT_SPECULAR:
			case MAT_SHININESS:
			case MAT_TRANSPARENCY:
			if(objetos_debug){debug_printf("DEBUG:    l_subchunk_id=%X\n",l_chunk_id);}
				l_subchunk_id=l_chunk_id;
                break;
                
                
			case MAT_SUB_RGB:
			if(objetos_debug){debug_printf("DEBUG:    MAT_SUB_RGB (l_subchunk_id=%X)\n",l_subchunk_id);}
				fread (l_rgb, 1, 3, l_file);
				if(objetos_debug>=2){debug_printf("DEBUG:     RGB(%hi,%hi,%hi)\n",l_rgb[0],l_rgb[1],l_rgb[2]);}
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
			if(objetos_debug){debug_printf("DEBUG:    MAT_SUB_PER (l_subchunk_id=%X)\n",l_subchunk_id);}
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
			if(objetos_debug){debug_printf("DEBUG:    MAT_TEX1\n");}
				break;
			
			case MAT_MAPNAME:
			if(objetos_debug){debug_printf("DEBUG:    MAT_MAPNAME\n");}
				i=0;
				do
				{
					fread (&l_char, 1, 1, l_file);
                    string[i]=(char)l_char;
					i++;
                }while(l_char != '\0' && i<80);
                if(i==80){debug_printf("ALERTA: Nombre de textura cortado. (No deber�a de haber pasado nunca, probablemente sea por un archivo corrupto)\n",string);}
                //str_append(string,".tga");
                str_cpy(string2,app_path);
                str_append(string2,"materials\\");
                str_append(string2,string);
                /* Cargamos la textura */
				data->material[data->materials_qty-1].texture[0]=ilutGLLoadImage(string2);
				if(objetos_debug>=2){debug_printf("DEBUG:     Material %i, textura cargada en %i\n",data->materials_qty-1,data->material[data->materials_qty-1].texture[0]);}
				if(!data->material[data->materials_qty-1].texture[0]){debug_printf("ALERTA: Textura autocargada del 3ds \"%s\" no encontrada.\n",string);}
				else
				{
				 	if(objetos_debug){debug_printf("INFO: Textura autocargada del 3ds \"%s\".\n",string);}
					ilutGLBuildMipmaps();
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				}
				break;
			
			
			/* Nos saltamos los desconocidos */
			default:
			if(objetos_debug){debug_printf("DEBUG:    SKIPPED!\n");}
				 fseek(l_file, l_chunk_lenght-6, SEEK_CUR);
        } 
	}
	fclose (l_file);
	return 0;
}


/*! \fn void object_predraw(t_obj_base_ptr object)
 *  \brief Dibuja un objeto mediante OpenGL en la lista correspondiente
 *  \param object Objeto a dibujar
*/

void object_predraw(t_obj_base *object)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	int l_index;
	glDisable(GL_LIGHTING);
	
	/*glBindTexture(GL_TEXTURE_2D, object.id_texture); ARREGLAR ESTO!, en teor�a depende de la cara en la que estemos */
	
	
	object->modelo->draw_list=glGenLists(1);
	object->modelo->draw_lists=1;
	
	glNewList(object->modelo->draw_list,GL_COMPILE);
	
    glDisable(GL_CULL_FACE);
    
	glColor3f(1.0f,1.0f,1.0f);
	glEnable(GL_TEXTURE_2D);
	
	for (l_index=0;l_index<object->modelo->polygons_qty;l_index++)
    {
	    glBindTexture(GL_TEXTURE_2D, object->modelo->polygon[l_index].texture);
		glBegin(GL_TRIANGLES);
			/* Primer v�rtice */
	        glTexCoord2f( object->modelo->mapcoord[ object->modelo->polygon[l_index].a ].u ,
	                      object->modelo->mapcoord[ object->modelo->polygon[l_index].a ].v );
	        glVertex3f( object->modelo->vertex[ object->modelo->polygon[l_index].a ].x *object->modelo->size,
	                    object->modelo->vertex[ object->modelo->polygon[l_index].a ].y *object->modelo->size,
	                    object->modelo->vertex[ object->modelo->polygon[l_index].a ].z *object->modelo->size);
	
	        /* Segundo v�rtice */
	        glTexCoord2f( object->modelo->mapcoord[ object->modelo->polygon[l_index].b ].u ,
	                      object->modelo->mapcoord[ object->modelo->polygon[l_index].b ].v );
	        glVertex3f( object->modelo->vertex[ object->modelo->polygon[l_index].b ].x *object->modelo->size,
	                    object->modelo->vertex[ object->modelo->polygon[l_index].b ].y *object->modelo->size,
	                    object->modelo->vertex[ object->modelo->polygon[l_index].b ].z *object->modelo->size);
	
	        /* Tercer v�rtice */
	        glTexCoord2f( object->modelo->mapcoord[ object->modelo->polygon[l_index].c ].u ,
	                      object->modelo->mapcoord[ object->modelo->polygon[l_index].c ].v );
	        glVertex3f( object->modelo->vertex[ object->modelo->polygon[l_index].c ].x *object->modelo->size,
	                    object->modelo->vertex[ object->modelo->polygon[l_index].c ].y *object->modelo->size,
	                    object->modelo->vertex[ object->modelo->polygon[l_index].c ].z *object->modelo->size);
	    glEnd();
    }
    glEnable(GL_CULL_FACE);
	glEndList();
			
	l_index=glGetError();
	if (l_index){debug_printf("ERROR: OpenGL ha retornado %i al llamar a object_predraw() - Esto no deber�a de haber pasado nunca, que raro...\n",l_index);}
	
	glPopMatrix();
}


/*! \fn void object_draw_l(t_obj_base object)
 *  \brief Dibuja un objeto mediante listas de OpenGL en las coordenadas correspondientes
 *  \param object Objeto a dibujar
 *	\bug Falta el roll del objeto
 *	\sa object_predraw
*/

void object_draw_l(t_obj_base_ptr object)
{
	int l_index;
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glEnable(GL_LIGHTING);
	
    glRotatef(object->rot.y, 0,0,1.0f);
    glRotatef(object->rot.x, 1.0f,0,0);
    glRotatef(object->rot.z, 0,1.0f,0);
    //glRotatef(object->rot.z, sin(RAD(object.yaw)),cos(RAD(object.yaw)),-sin(RAD(object.pitch)));
    
    glTranslatef(object->pos.x,object->pos.y,object->pos.z);
    
    glCallList(object->modelo->draw_list);
	glPopMatrix();
}

void model_unload(t_model_ptr model)
{
	int i;
	glDeleteLists(model->draw_list,1);
	model->draw_list=0;
	ilBindImage(0);
	ilDeleteImages(model->materials_qty,model->material[i].texture);
	for (i=0; i<model->materials_qty; i++)
	{
		model->material[i].texture[0]=0;
	}
}

/*! \fn void object_draw(t_obj_base object)
 *  \brief Dibuja un objeto mediante OpenGL en las coordenadas correspondientes
 *  \param object Objeto a dibujar
 *	\bug Falta el roll del objeto
 *	\deprecated Mejor usar listas que esta funci�n
 *	\sa object_draw_l
*/

void object_draw(t_obj_base object)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	int l_index;
	glEnable(GL_LIGHTING);
	
	glColor3f(1.0f,1.0f,1.0f);
	glEnable(GL_TEXTURE_2D);

    glRotatef(object.rot.y, 0,0,1.0f);
    glRotatef(object.rot.x, 1.0f,0,0);
    /*glRotatef(object.rot.z, sin(RAD(object.yaw)),cos(RAD(object.yaw)),-sin(RAD(object.pitch))); ARREGLAR!!!*/
    
    
    glTranslatef(object.pos.x,object.pos.y,object.pos.z);

    glBegin(GL_TRIANGLES);
	for (l_index=0;l_index<object.modelo->polygons_qty;l_index++)
    {
        glBindTexture(GL_TEXTURE_2D, object.modelo->polygon[l_index].texture);
		/* Primer v�rtice */
        glTexCoord2f( object.modelo->mapcoord[ object.modelo->polygon[l_index].a ].u ,
                      object.modelo->mapcoord[ object.modelo->polygon[l_index].a ].v );
        glVertex3f( object.modelo->vertex[ object.modelo->polygon[l_index].a ].x *object.modelo->size,
                    object.modelo->vertex[ object.modelo->polygon[l_index].a ].y *object.modelo->size,
                    object.modelo->vertex[ object.modelo->polygon[l_index].a ].z *object.modelo->size);

        /* Segundo v�rtice */
        glTexCoord2f( object.modelo->mapcoord[ object.modelo->polygon[l_index].b ].u ,
                      object.modelo->mapcoord[ object.modelo->polygon[l_index].b ].v );
        glVertex3f( object.modelo->vertex[ object.modelo->polygon[l_index].b ].x *object.modelo->size,
                    object.modelo->vertex[ object.modelo->polygon[l_index].b ].y *object.modelo->size,
                    object.modelo->vertex[ object.modelo->polygon[l_index].b ].z *object.modelo->size);

        /* Tercer v�rtice */
        glTexCoord2f( object.modelo->mapcoord[ object.modelo->polygon[l_index].c ].u ,
                      object.modelo->mapcoord[ object.modelo->polygon[l_index].c ].v );
        glVertex3f( object.modelo->vertex[ object.modelo->polygon[l_index].c ].x *object.modelo->size,
                    object.modelo->vertex[ object.modelo->polygon[l_index].c ].y *object.modelo->size,
                    object.modelo->vertex[ object.modelo->polygon[l_index].c ].z *object.modelo->size);
	}
	glEnd();
	glPopMatrix();
}


/*! \fn t_obj_base_ptr *lista_base_crear(int* contador)
 *  \brief Crea una lista de objetos base
 *  \param contador Contador de la lista
 *	\return lista con su malloc establecido
 *	\return Si contador <0 ha habido un error en el malloc
*/

/*t_obj_base **lista_base_crear(int* contador)
{
	t_obj_base **lista;
	(*contador)=0;
	lista=malloc(sizeof(t_obj_base*));
    if (lista==NULL){*contador=-1;}
    lista[0]=NULL;
    return lista;
}*/


/*! \fn int lista_base_aumentar(t_obj_base_ptr *lista, int* contador, t_obj_base_ptr elemento)
 *  \brief A�ade a una lista de objetos tipo base un elemento
 *  \param lista La lista a la cual se debe a�adir el elemento
 *  \param contador Contador de la lista
 *  \param elemento Puntero al elemento a a�adir a la lista
 *	\return 0 si todo fu� bien.
*/

/*int lista_base_aumentar(t_obj_base **lista, int* contador, t_obj_base *elemento)
{
    lista[*contador]=elemento;
	(*contador)++;
	lista=realloc(lista, sizeof(t_obj_base*)*((*contador)+1));
    if (lista==NULL){(*contador)=-1;return -1;}
    lista[(*contador)]=NULL;
    return 0;
}*/


/*! \fn int lista_base_crear_elemento(t_obj_base_ptr *lista, int* contador, char *ruta)
 *  \brief Carga un objeto tipo base y lo a�ade a la lista.
 *  \param lista La lista a la cual se debe a�adir el elemento
 *  \param contador Contador de la lista
 *  \param ruta Ruta al archivo 3ds
 *	\return 0 si todo fu� bien.
 *	\return 1 Archivo no encontrado.
 *	\return -1 Error interno del archivo 3ds.
 *	\return -2 Error en el malloc del objeto base.
 *	\return -3 Error en el malloc del modelo.
*/

int lista_base_crear_elemento(t_obj_base **lista, int *contador, char *ruta)
{
	t_obj_base *objeto; // Objetos que vamos a guardar en memoria
	t_model *modelo;
	
	objeto = malloc(sizeof(t_obj_base));
	if (objeto==NULL){return -2;}
	modelo = malloc(sizeof(t_model));
	if (modelo==NULL){free(objeto); return -3;}
	
	
	int i;
	i=load_3DS(modelo, ruta);
	objeto->modelo=modelo;
	
	if (i!=0){debug_printf("Error al cargar el modelo 3DS \"%s\", retornado %i.\n",ruta,i); free(objeto); free(modelo); return i;}
    obj_ptr_setpos(objeto,0,0,0);
	object_predraw(objeto);
	
	//lista=realloc(lista, sizeof(t_obj_base*)*((*contador)+1));
	//if (lista==NULL){debug_printf("Error en el realloc de la lista de objetos\n"); free(objeto); free(modelo); *contador=-1; return -1;}
    
	
    lista[*contador]=objeto;
	(*contador)++;
	
	
    return 0;
}

/*! \fn int lista_base_borrar_elemento(t_obj_base_ptr *lista, int* contador, unsigned int index)
 *  \brief Borra de una lista de objetos base un elemento
 *  \warning Esta funci�n no borra de la memoria el objeto eliminado de la lista, se debe hacer manualmente
 *  \param lista La lista de la cual se debe borrar el elemento
 *  \param contador Contador de la lista
 *  \param index Elemento a borrar
 *	\return 0 si todo fu� bien.
*/

/*int lista_base_borrar_elemento(t_obj_base **lista, int* contador, unsigned int index)
{
	(*contador)--;
	int i;
	for (i=index; i<*contador; i++){lista[i]=lista[i+1];}
	lista=realloc(lista, sizeof(t_obj_base_ptr)*(*contador+1));
    if (lista==NULL){*contador=-1;return -1;}
    return 0;
}*/

/*! \fn void lista_base_limpiar(t_obj_base_ptr *lista, int* contador)
 *  \brief Limpia de la memoria todos los elementos de la lista y borra la lista
 *  \warning No llamar a la funci�n lista_base_borrar ni intentar dibujar la lista despu�s de usar esta funci�n.
 *  \param lista La lista que contiene los elementos que se deben borrar
 *  \param contador Contador de la lista
*/

void lista_base_limpiar(t_obj_base **lista, int* contador)
{
	*contador--;
    while(*contador>=0)
	{
		model_unload(lista[*contador]->modelo);
		free(lista[*contador]->modelo);
		free(lista[*contador]);
		(*contador)--;
	}
	lista_base_borrar(lista, contador);
}


/*! \fn void lista_base_borrar(t_obj_base_ptr *lista, int* contador)
 *  \brief Limpia de la memoria una lista de objetos base
 *  \warning No llamar a la funci�n lista_base_borrar despu�s de usar lista_base_limpiar.
 *	Esta funci�n se debe usar solo si se limpia de la memoria los objetos de forma manual
 *  \param lista La lista que se debe borrar
 *  \param contador Contador de la lista
*/

void lista_base_borrar(t_obj_base **lista, int* contador)
{
	//free(lista);
	(*contador)=0;
	//lista=NULL;
}
