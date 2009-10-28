#include "heightmap.h"

static int str_size(const char* string)
{
	int i=0;
	while(string[i]!=0){i++;}
	return i;
}
static void str_cpy(char* string1,const char* string2)
{
	int i=0;
	while(string2[i]!=0){string1[i]=string2[i];i++;}
	string1[i]=0;
}
static void str_append(char* string1,const char* string2)
{
	int i=str_size(string1);
	int j=0;
	while(string2[j]!=0){string1[i]=string2[j];i++;j++;}
	string1[i]=0;
}
static VECTOR p_vect(VECTOR vec1,VECTOR vec2)
{
	VECTOR resultado;
	resultado.x = vec1.y*vec2.z -vec1.z*vec2.y;
	resultado.y = vec1.z*vec2.x -vec1.x*vec2.z;
	resultado.z = vec1.x*vec2.y -vec1.y*vec2.x;
	return resultado;
}
static void normalize(VECTOR* vec1)
{
	if (vec1->x||vec1->y||vec1->z)
	{
		float modulo = sqrt(vec1->x*vec1->x + vec1->y*vec1->y + vec1->z*vec1->z);
		vec1->x/=modulo;
		vec1->y/=modulo;
		vec1->z/=modulo;
	}
}
static VECTOR calc_normal(VECTOR vec1,VECTOR vec2)
{
	VECTOR resultado;
	resultado=p_vect(vec1,vec2);
	normalize(&resultado);
	return resultado;
}

int load_heightmap(const char* filename, t_heightmap* h_buffer)
{
	char str_buffer[255];/* Añade la extensión */
	
	h_buffer->list=0;
	
	str_cpy(str_buffer,filename);
	str_append(str_buffer,".pgm");
	FILE* data = fopen(str_buffer,"r"); if (!data){debug_printf("### Fallo al abrir \"%s\"",str_buffer);return 1;}
	
	str_cpy(str_buffer,filename);
	str_append(str_buffer,".txt");
	FILE* info = fopen(str_buffer,"r"); if (!info){debug_printf("### Fallo al abrir \"%s\"",str_buffer);return 2;}
	
	/* Cargamos los datos básicos */
	// Hay que currarse un cargador decente no tan secuencial...
	fscanf(info,"North\t= %f\n",&(h_buffer->north));
	fscanf(info,"South\t= %f\n",&(h_buffer->south));
	fscanf(info,"East\t= %f\n",&(h_buffer->east));
	fscanf(info,"West\t= %f\n",&(h_buffer->west));
	fscanf(info,"Scale\t= %f\n",&(h_buffer->scale));
	fscanf(info,"Lines\t= %i\n",&(h_buffer->tam_y));
	fscanf(info,"Samples\t= %i\n",&(h_buffer->tam_x));
	fscanf(info,"zero_h\t= %i\n",&(h_buffer->zero_h));
	fscanf(info,"min_h\t= %i\n",&(h_buffer->min_h));
	fscanf(info,"max_h\t= %i\n",&(h_buffer->max_h));
	fscanf(info,"ini_x\t= %i\n",&(h_buffer->ini_x));
	fscanf(info,"ini_y\t= %i\n",&(h_buffer->ini_y));
	fscanf(info,"ini_z\t= %i\n",&(h_buffer->ini_z));
	fclose(info);
	
	h_buffer->data=malloc(sizeof(int)*h_buffer->tam_y*h_buffer->tam_x);
	if(!h_buffer->data){debug_printf("### Fallo en el malloc (%i*%i)",h_buffer->tam_y,h_buffer->tam_x);return 3;}
	
	int i=0;
	char val[2];
	int vali[2];
	char line[255];
	fscanf(data,"%c%c\n",&(val[0]),&(val[1])); if(val[0]!='P'||val[1]!='2'){debug_printf("### Fallo, no es un PGM (%c%c)",val[0],val[1]);return 4;}
	
	do{fgets(line,255,data);}
	while(line[0]=='#');/* Leemos una línea. Si hay un '#' implica que era un comentario... */
	fscanf(data,"%i\n",&i);
	i=0;
	while(fscanf(data,"%i\n",&(h_buffer->data[i]))!=EOF && i<h_buffer->tam_y*h_buffer->tam_x){i++;}
	fclose(data);
	return 0;
}


void destroy_heightmap(t_heightmap* obj)
{
	free(obj->data);
	glDeleteLists(obj->list,1);
}


void create_map(t_heightmap* obj, t_texture texture)
{
	int x,y;
	int half_x, half_y;
	half_x = obj->tam_x/2;
	half_y = obj->tam_y/2;
	
	float v_scale = (obj->max_h-obj->min_h)/255.0f;
	float v_add = -(obj->data[half_x+half_y*obj->tam_x]-obj->zero_h)*v_scale;
	/*
	- NOTA - No calculo al final las normales porque hace muuucho lag!
	VECTOR normal;
	VECTOR vec1, vec2, vec3;/* Vectores que forman las aristas de los triángulos para calcular las normales *//*
	VECTOR pos1, pos2;/* Posición de los puntos anteriores */
	
			/* - VECTORES -
			  pos2    (x,y+1,z')
			    o --- o
			    | \   |
			    |  \  v3
			    |  v2 |
			    |    \|
			    o-v1--o
			  pos1    (x,y,z)
			*/
	
	obj->list=glGenLists(1);
	glNewList(obj->list,GL_COMPILE);
		use_texture(texture);
		
		glPushMatrix();
		glTranslatef(-obj->ini_x,-obj->ini_y,-obj->ini_z);
		glRotatef(180, 0, 0, 1.0f);
		
		for (y=0;y<obj->tam_y; y++)
		{
			glEnable(GL_TEXTURE_2D);
			glColor3d(1.0f,1.0f,1.0f);
			glBegin(GL_TRIANGLE_STRIP);
			/* TODO: Simular sombras!! */
			for (x=obj->tam_x-1;x>=0; x--)
			{
				/* Si no es el primero */
				/*if (x!=obj->tam_x-1)
				{
					vec1.x=x-pos1.x;
					vec1.y=y-pos1.y;
					vec1.z=obj->data[obj->tam_x-x-1+y*obj->tam_x]-pos1.z;
					
					vec2.x=x-pos2.x;
					vec2.y=y-pos2.y;
					vec2.z=obj->data[obj->tam_x-x-1+y*obj->tam_x]-pos2.z;
					
					vec3.x=0;
					vec3.y=1;
					vec3.z=obj->data[obj->tam_x-x-1+(y+1)*obj->tam_x]-obj->data[obj->tam_x-x-1+y*obj->tam_x];/* (x,y+1,z)-(x,y,z') */
					/*
					normal=calc_normal(vec1,vec2);
					glNormal3f( normal.x, normal.y, normal.z);
				}
				/* Guardamos las posiciones *//*
				pos1.x=x;pos1.y=y;pos1.z=obj->data[obj->tam_x-x-1+y*obj->tam_x];
				pos2.x=x;pos2.y=y+1;pos2.z=obj->data[obj->tam_x-x-1+(y+1)*obj->tam_x];
				*/
				/* Dibujamos el primer triángulo */
				glTexCoord2i( (x - half_x)*obj->scale*1000.0f, (y - half_y)  *obj->scale*1000.0f);
				glVertex3f  ( (x - half_x)*obj->scale*1000.0f, (y - half_y)  *obj->scale*1000.0f, (obj->data[obj->tam_x-x-1+y*obj->tam_x]-obj->zero_h)*v_scale+v_add);
				
				/* Calculamos la normal del segundo */
				/*if (x!=obj->tam_x-1){normal=calc_normal(vec3,vec2);glNormal3f( normal.x, normal.y, normal.z);}
				/* Dibujamos el segundo triángulo */
				glTexCoord2i( (x - half_x)*obj->scale*1000.0f, (y+1 - half_y)*obj->scale*1000.0f);
				glVertex3f  ( (x - half_x)*obj->scale*1000.0f, (y+1 - half_y)*obj->scale*1000.0f, (obj->data[obj->tam_x-x-1+(y+1)*obj->tam_x]-obj->zero_h)*v_scale+v_add);
			}
			glEnd();
			/* Cuadrícula (Triángula? xD) *//*
			glDisable(GL_TEXTURE_2D);
			glBegin(GL_LINE_STRIP);
			glColor3d(0.0f,0.0f,0.0f);
			for (x=obj->tam_x-1;x>=0; x--)
			{
				glVertex3f  ( (x - half_x)*obj->scale*1000.0f, (y - half_y)  *obj->scale*1000.0f, (obj->data[obj->tam_x-x-1+y*obj->tam_x]-obj->zero_h)*v_scale+v_add);
				glVertex3f  ( (x - half_x)*obj->scale*1000.0f, (y+1 - half_y)*obj->scale*1000.0f, (obj->data[obj->tam_x-x-1+(y+1)*obj->tam_x]-obj->zero_h)*v_scale+v_add);
			}
			glEnd();*/
		}
		glPopMatrix();
	glEndList();
}
