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

/** \file shared.c
 * \brief Funciones compartidas por todos los módulos
 * Este archivo contiene todo el código de las funciones que son necesarias por todos los
 * diferentes módulos del programa. Está formado por funciones de bajo nivel personalizadas para la aplicación.
 * \author Alfonso Arbona Gimeno
*/

#include "shared.h"

/* - MATH - */
float nabs(float val)
{
	if (val<0){return -val;}
	return val;
}

int nround(float val)
{
	int buff=val;
	if(val-buff>=0.5f){return buff+1;}
	return buff;
}

int nfloor(float val)
{
	return (int)val;
}

int nceil(float val)
{
	int buff=(int)val;
	if(val-buff!=0.0f){return (int)val+1;}
	return val;
}

/* - SCREEN LIST PRINTF - */
void scr_init_reset (int debug_too)
{
	scr_message_index=-1;
	scr_message_debug=debug_too;
	
}

static void scr_init_push (void)
{
	int i;
	if (scr_message_index>0)
	{
		for (i=0; i<scr_message_index-1; i++)
		{
			str_cpy(scr_messages[i], scr_messages[i+1]);
		}
		scr_message_index--;
	}
}

void scr_init_printf (const char *fmt, ...)
{
	char buf[MAX_SCREEN_MESSAGES_LEN];
	va_list args;
	va_start(args, fmt);
	vsprintf (buf, fmt, args);
	va_end(args);
	if (scr_message_index==MAX_SCREEN_MESSAGES-1){scr_init_push();}
	str_cpyl(scr_messages[scr_message_index],MAX_SCREEN_MESSAGES_LEN,buf);
	scr_message_index++;
	/* Printing */
	if (scr_message_debug){debug_printf(scr_messages[scr_message_index-1]);}
	int i;
	int pos=0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (i=scr_message_index-1; i>=0; i--)
	{
		hud_printf (1, pos, scr_messages[i]);
		pos++;
	}
	glFinish();
	glutSwapBuffers();
}

void scr_init_reprintf (const char *fmt, ...)
{
	char buf[MAX_SCREEN_MESSAGES_LEN];
	va_list args;
	va_start(args, fmt);
	vsprintf (buf, fmt, args);
	va_end(args);
	scr_message_index--;
	if (scr_message_index==MAX_SCREEN_MESSAGES-1){scr_init_push();}
	str_cpyl(scr_messages[scr_message_index],MAX_SCREEN_MESSAGES_LEN,buf);
	scr_message_index++;
	/* Printing */
	if (scr_message_debug){debug_printf(scr_messages[scr_message_index-1]);}
	int i;
	int pos=0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (i=scr_message_index-1; i>=0; i--)
	{
		hud_printf (1, pos, scr_messages[i]);
		pos++;
	}
	glFinish();
	glutSwapBuffers();
}



/* - STRINGS - */

/*! \fn int str_size(const char* string)
 *  \brief Obtiene el número de carácteres de un string.
 *  \param string String a medir
 *	\return Número de carácteres del string (Sin el \\0 final)
*/
int str_size(const char* string)
{
	int i=0;
	while(string[i]!=0){i++;}
	return i;
}

/*! \fn void str_cpy(char* string1,const char* string2)
 *  \brief Copia el contenido de string2 a string1
 *  \param string1 String de destino
 *  \param string2 String de origen
 *	\sa str_cpyl
*/
void str_cpy(char* string1,const char* string2)
{
	int i=0;
	while(string2[i]!=0){string1[i]=string2[i];i++;}
	string1[i]=0;
}

/*! \fn void str_cpyl(char* string1, int len, const char* string2)
 *  \brief Copia el contenido de string2 a string1 con un máximo de 'len' carácteres
 * 	Es mucho más recomendable usar esta función para evitar salirnos del tamaño del string.
 * 	Siempre acaba con \\0, incluso cuando no hay espacio.
 *  \param string1 String de destino
 *  \param string2 String de origen
 *	\sa str_cpy
*/
void str_cpyl(char* string1, int len, const char* string2)
{
	int i=0;
	while(string2[i]!=0 && i<len){string1[i]=string2[i];i++;}
	string1[i]=0;
}

/*! \fn void str_append(char* string1,const char* string2)
 *  \brief Añade el contenido de string2 a string1
 * 	Hay que asegurarse de que hay suficiente espacio en string1 para ambos strings
 *  \param string1 String de destino
 *  \param string2 String de origen
*/
void str_append(char* string1,const char* string2)
{
	int i=str_size(string1);
	int j=0;
	while(string2[j]!=0){string1[i]=string2[j];i++;j++;}
	string1[i]=0;
}

/*! \fn int str_cmp(const char* string1,const char* string2)
 *  \brief Compara el contenido de ambos strings
 *  \warning Si los strings son distintos el resultado retornado es la resta de
 *	los primeros carácteres distintos. Creo que es distinto de la función de strings.h
 *  \param string1 String1
 *  \param string2 String2
 *  \return 0 si son iguales.
*/
int str_cmp(const char* string1,const char* string2)
{
	int valor = 0;
	int pos=0;
	while (valor == 0 && string1[pos]!=0 && string2[pos]!=0)
	{
		valor=string1[pos]-string2[pos];
		pos++;
	}
	return valor;
}

/*! \fn void str_ruta_back(char* ruta)
 *  \brief Borra el texto empezando desde el final hasta el primer '\' (El '\' se mantiene)
 *  \param ruta Ruta a la que se va a borrar el final
*/
void str_ruta_back(char* ruta)
{
	int pos=str_size(ruta);
	while (pos>=0 && ruta[pos]!='\\'){pos--;}
	if(pos>0){ruta[pos+1]=0;}
}

/*! \fn int str_list_find(const char* find,const char* table[], int elementos)
 *  \brief Busca un string en una lista de strings y retorna la posición en la lista
 *  \param find Texto a encontrar
 *  \param table Lista de strings donde se debe buscar el texto
 *  \param elementos Número de elementos (o filas) en la lista 'table'
 *  \return -1 si no se encuentra el texto
 *  \return La fila en la que se encuentra el texto. (Empezando por 0)
*/
int str_list_find(const char* find,const char* table[], int elementos)
{
	int i;
	for (i=0; i<elementos; i++)
	{
		if (!str_cmp(find, table[i])){return i;}
	}
	return -1;
}

/* - VECTORS - */

/*! \fn VECTOR p_vect(VECTOR vec1,VECTOR vec2)
 *  \brief Calcula el producto vectorial de dos vectores
 *  \param vec1 Vector 1
 *  \param vec2 Vector 2
 *  \return El vector resultante del producto vectorial (Perpendicular a ambos vectores)
*/
VECTOR p_vect(VECTOR vec1,VECTOR vec2)
{
	VECTOR resultado;
	resultado.x = vec1.y*vec2.z -vec1.z*vec2.y;
	resultado.y = vec1.z*vec2.x -vec1.x*vec2.z;
	resultado.z = vec1.x*vec2.y -vec1.y*vec2.x;
	return resultado;
}

/*! \fn float p_escalar(VECTOR vec1,VECTOR vec2)
 *  \brief Calcula el producto escalar de dos vectores
 *  \param vec1 Vector 1
 *  \param vec2 Vector 2
 *  \return El resultado del producto escalar de dos vectores (Proyección)
*/
float p_escalar(VECTOR vec1,VECTOR vec2)
{
	return vec1.x*vec2.x + vec1.y*vec2.y + vec1.z*vec2.z;
}

/*! \fn void normalize(VECTOR* vec1)
 *  \brief Normaliza el vector. (Unitario)
 *  \param vec1 Vector a normalizar (También es el vector de retorno)
*/
void normalize(VECTOR* vec1)
{
	if (vec1->x||vec1->y||vec1->z)
	{
		float modulo = sqrt(vec1->x*vec1->x + vec1->y*vec1->y + vec1->z*vec1->z);
		vec1->x/=modulo;
		vec1->y/=modulo;
		vec1->z/=modulo;
	}
}

/*! \fn VECTOR vadd(VECTOR vec1, VECTOR vec2)
 *  \brief Calcula la suma de dos vectores
 *  \param vec1 Vector 1
 *  \param vec2 Vector 2
 *  \return vec1+vec2
*/
VECTOR vadd(VECTOR vec1, VECTOR vec2)
{
	VECTOR res;
	res.x=vec1.x+vec2.x;
	res.y=vec1.y+vec2.y;
	res.z=vec1.z+vec2.z;
	return res;
}

/*! \fn VECTOR vsub(VECTOR vec1, VECTOR vec2)
 *  \brief Calcula la resta de dos vectores
 *  \param vec1 Vector 1
 *  \param vec2 Vector 2
 *  \return vec1-vec2
*/
VECTOR vsub(VECTOR vec1, VECTOR vec2)
{
	VECTOR res;
	res.x=vec1.x-vec2.x;
	res.y=vec1.y-vec2.y;
	res.z=vec1.z-vec2.z;
	return res;
}

/* - DEBUG - */

/*! \fn void debug_reset(void)
 *  \brief Resetea el archivo debug.log
 *  Se debe llamar a esta función antes de usar las funciones de debug.
 *	Si el archivo debug.log no existe lo crea.
 *	Escribe en el archivo: " --- DEBUG --- \\n\\n"
*/
void debug_reset(void)
{
    FILE* file;
    file=fopen("debug.log","w");
    fprintf(file," --- DEBUG --- \n\n");
    fclose(file);
}

/*! \fn void debug_printf(const char *fmt, ...)
 *  \brief Escribe en el archivo debug.log como un fprintf
*/
void debug_printf(const char *fmt, ...)
{
    static char buf[256];
    FILE* file;
    file=fopen("debug.log","a");
    va_list args;
    va_start(args, fmt);
    vfprintf (file, fmt, args);
    va_end(args);
    fclose(file);
}

/* - PRINTING - */

/*! \fn void position_printf(float x, float y, float z, const char *fmt, ...)
 *  \brief Muestra texto en el mundo en 3D al estilo de printf
 *  \param x,y,z Posición del texto en el mundo
 *	Esta es una función muy básica ya que no permite girar ni cambiar el tamaño del texto.
*/
void position_printf(float x, float y, float z, const char *fmt, ...)
{
	static char buf[256];
    int viewport[4];
    void *font = GLUT_STROKE_ROMAN;
    va_list args;

    va_start(args, fmt);
    (void) vsprintf (buf, fmt, args);
    va_end(args);
    
	glDisable(GL_TEXTURE_2D);
	
    glGetIntegerv(GL_VIEWPORT,viewport);

    //glRotatef(90, 1.0f,0,0);
    glTranslatef(x+1,y+1,z-1);


    //glPushMatrix();
    //glLoadIdentity();

    //glMatrixMode(GL_PROJECTION);
    //glPushMatrix();
    //glLoadIdentity();

        glOrtho(0,viewport[2],0,viewport[3],-1,1);
        
        glColor3d(1.0,1.0,1.0);
        /*glRasterPos2i(
              glutBitmapWidth(font, ' ') * col,
            - glutBitmapHeight(font) * (row+1) + viewport[3]
        );*/
        glutStrokeString (font, (unsigned char *) buf);
        //glColor3d(1.0,1.0,1.0);

    //glPopMatrix();
    //glMatrixMode(GL_MODELVIEW);
    //glPopMatrix();
}

/*! \fn void hud_printf(int col, int row, const char *fmt, ...)
 *  \brief Muestra texto en el HUD (2D) al estilo de printf
 *  \param col Columna en la que mostrar el texto
 *  \param row Fila en la que mostrar el texto
 *	Esta es una función muy básica ya que no permite cambiar el color del texto.
*/
void hud_printf(int col, int row, const char *fmt, ...)
{
    
	glDisable(GL_LIGHTING);
	glDisable(GL_FOG);
	glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
	static char buf[256];
    int viewport[4];
    //void *font = GLUT_BITMAP_HELVETICA_12;
    void *font = GLUT_BITMAP_9_BY_15;
    va_list args;

    va_start(args, fmt);
    (void) vsprintf (buf, fmt, args);
    va_end(args);
    
	glDisable(GL_TEXTURE_2D);
	
    glGetIntegerv(GL_VIEWPORT,viewport);

    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

        glOrtho(0,viewport[2],0,viewport[3],-1,1);
        
        glColor3d(1.0,1.0,1.0);
        glRasterPos2i(
              glutBitmapWidth(font, ' ') * col,
            - glutBitmapHeight(font) * (row+1) + viewport[3]
        );
        glutBitmapString (font, (unsigned char *) buf);
        glColor3d(1.0,1.0,1.0);

    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_FOG);
}

/* - Other - */

/*! \fn void use_texture(t_texture texture)
 *  \brief glBindTexture para materiales
 *  \param texture Material a usar en OpenGL
*/
void use_texture(t_texture texture)
{
	glEnable(GL_TEXTURE_2D);
	glMaterialfv(GL_FRONT, GL_AMBIENT,   texture.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,   texture.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR,  texture.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, texture.shininess);
	glBindTexture(GL_TEXTURE_2D, texture.texture[0] );
}

/*! \fn void draw_sprite (float x, float y, float z, t_texture textura, float size)
 *  \brief Dibuja una textura en el mundo siempre orientada al jugador.
 *  \param x,y,z Coordenadas donde se dibuja el objeto
 *  \param textura Material a dibujar
 *  \param size Tamaño del objeto (Radio del objeto o mitad de su lado)
*/
void draw_sprite (float x, float y, float z, t_texture textura, float size)
{
	float proy_x, proy_y, proy_z, mod;
	mod = sqrt((x-camera.pos_x)*(x-camera.pos_x)+(y-camera.pos_y)*(y-camera.pos_y)+(z-camera.pos_z)*(z-camera.pos_z));
	if (mod==0)return;
	proy_x = (x-camera.pos_x)/mod;
	proy_y = (y-camera.pos_y)/mod;
	proy_z = (z-camera.pos_z)/mod;
	
	use_texture(textura);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x-size*proy_y, y+size*proy_x, z-size);// Bottom Left
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+size*proy_y, y-size*proy_x, z-size);// Bottom Right
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+size*proy_y, y-size*proy_x, z+size);// Top Right
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x-size*proy_y, y+size*proy_x, z+size);// Top Left
	glEnd();
}

/*! \fn void draw_fixsprite (float x, float y, float z, t_texture textura, float size)
 *  \brief Dibuja una textura en el mundo siempre orientada al jugador pero fija (como por ejemplo el Sol).
 *  \param x,y,z Coordenadas donde se dibuja el objeto
 *  \param textura Material a dibujar
 *  \param size Tamaño del objeto (Radio del objeto o mitad de su lado)
*/
void draw_fixsprite (float x, float y, float z, t_texture textura, float size)
{
	use_texture(textura);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x-size*(sin(RAD(camera.yaw+90))), y+size*(cos(RAD(camera.yaw+90))-cos(RAD(camera.pitch))), z-size*sin(RAD(camera.pitch)));// Bottom Left
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+size*(sin(RAD(camera.yaw+90))), y-size*(cos(RAD(camera.yaw+90))+cos(RAD(camera.pitch))), z-size*sin(RAD(camera.pitch)));// Bottom Right
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+size*(sin(RAD(camera.yaw+90))), y-size*(cos(RAD(camera.yaw+90))-cos(RAD(camera.pitch))), z+size*sin(RAD(camera.pitch)));// Top Right
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x-size*(sin(RAD(camera.yaw+90))), y+size*(cos(RAD(camera.yaw+90))+cos(RAD(camera.pitch))), z+size*sin(RAD(camera.pitch)));// Top Left
	glEnd();
}
