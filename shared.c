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
#include <GL/glu.h>
#include <GL/glext.h>
#include <IL/ilut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "shared.h"

void SDL_GL_RenderText(char *text, TTF_Font *font, SDL_Color color, float x, float y, float z)
{
	glEnable(GL_TEXTURE_2D);
	
	SDL_Surface *Message = TTF_RenderText_Blended(font, text, color);
	unsigned Texture = 0;
	
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Message->w, Message->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, Message->pixels);
	
	glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
	
	int viewport[4];
    glGetIntegerv(GL_VIEWPORT,viewport);
	glOrtho(0,viewport[2],0,viewport[3],-1,1);
	glColor3f(1,1,1);
	
	glBegin(GL_QUADS);
		glTexCoord2d(0, 1); glVertex3d(x,				scr_height-y,				z);
		glTexCoord2d(1, 1); glVertex3d(x+Message->w,	scr_height-y,				z);
		glTexCoord2d(1, 0); glVertex3d(x+Message->w,	scr_height-y+Message->h,	z);
		glTexCoord2d(0, 0); glVertex3d(x,				scr_height-y+Message->h,	z);
		
		glColor3f(1,0,0);
		glVertex3d(x,				scr_height-y,				z);
		glVertex3d(x+Message->w,	scr_height-y,				z);
		glVertex3d(x+Message->w,	scr_height-y+Message->h,	z);
		glVertex3d(x,				scr_height-y+Message->h,	z);
	glEnd();
	
	glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
	
	/* Bad things happen if we delete the texture before it finishes */
	glFinish();
	glDeleteTextures(1, &Texture);
	SDL_FreeSurface(Message);
}


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
	if(val-buff!=0.0f){return (int)val+nsgn(val);} /* Usando nsgn funciona incluso con negativos */
	return val;
}

int nsgn(float val)
{
	if (val>0)return 1;
	if (val<0)return -1;
	return 0;
}

float str2float(const char *str)
{
      float valor=0;
      int negativo=1;
      float decimal=0.0;/*Necesito tipo float porque la división me sale entera y sin decimales con el tipo integer*/
      while (((*str>=0x30 && *str<=0x39) || *str==0x2E || *str==0x2C || *str==45) && *str!='\0') /*Entre 0 y 9 ó '.' ó ',' y no es el final*/
      {
            if (*str==45){negativo=-1;}
            else
            {
                if (*str>=0x30 && *str<=0x39)
                {
                   if (decimal==0)
                   {
                       valor*=10;
                       valor+=((*str)-0x30);
                   }else{
                         valor+=(*str-0x30)/decimal;
                         decimal*=10;
                   }
                }else{decimal=10;}
            }
            str++;
            
      }
      return valor*negativo;
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
typedef struct st_Dialog
{
	int (*df)(int msg, struct st_Dialog * d, int c);
	int x, y;
	int w, h;
	COLORf fg, bg;
	int key;
	int flag;
	int d1, d2;
	void *dp, *dp2, *dp3;
}DIALOG;
extern int d_image_proc(int msg, struct st_Dialog * d, int c);
extern int draw_element(DIALOG d);
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (config.show_fondo)
	{
		/* DIALOG:         {(*df),        x, y, w,                  h,                fg,              bg, key, flag,            d1, d2, *dp, *dp2, *dp3} */
		DIALOG fondo_dlg = {d_image_proc, 0, 0, scr_width, scr_height, {128,128,128,128}, {128,128,128,128}, 0, 0, fondo.texture[0], 0, NULL, NULL, NULL};
		draw_element(fondo_dlg);
	}
	
	if (scr_message_debug)
	{
		char msg[MAX_SCREEN_MESSAGES_LEN];
		str_cpy(msg, "*[");
		str_append(msg, scr_messages[scr_message_index-1]);
		str_append(msg, "]*\n");
		debug_printf(msg);
	}
	int i;
	int pos=1;
	for (i=scr_message_index-1; i>=0; i--)
	{
		hud_printf (12, pos*12 + 12, scr_messages[i]);
		pos++;
	}
	glFinish();
	SDL_GL_SwapBuffers();
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
	int pos=1;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	for (i=scr_message_index-1; i>=0; i--)
	{
		hud_printf (12, pos*12, scr_messages[i]);
		pos++;
	}
	
	glFinish();
	SDL_GL_SwapBuffers();
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
 *  \param len Longitud máxima a copiar. Siempre acaba en \0, aunque corte parte del texto.
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

/*! \fn int str_ext_cmp(const char* ruta, const char* ext)
 *  \brief Compara la extensión del archivo señalado por ruta con ext.
 *  Básicamente coge de ruta el final hasta un '.' y compara eso con ext
 *  \warning Si los strings son distintos el resultado retornado es la resta de
 *	los primeros carácteres distintos. Creo que es distinto de la función de strings.h
 *  \warning NO retorna lo mismo que las funciones de cmp!!!. Aquí es 1 si son iguales, 0 si no lo son
 *  \param ruta El archivo a comparar (terminado en .algo)
 *  \param ext Extensión a comparar.
 *  \return 1 si son iguales.
 *  \return 0 si son distintos.
*/
int str_ext_cmp(const char* ruta, const char* ext)
{
	char file_ext[256]; // La extensión sacada de ruta
	
	int size_ruta = str_size(ruta);
	int pos=size_ruta;
	int cont=0;
	
	while (pos>=0 && ruta[pos]!='.'){pos--;}
	if(pos<=0){return -1;} // No tiene extensión
	
	pos++; // Ahora apunta al primer caracter después del '.'
	
	while(pos<size_ruta){file_ext[cont]=ruta[pos]; cont++; pos++;}
	file_ext[cont]=0; // Nos aseguramos... aunque no debería de haber ningún problema porque ruta ya tiene \0 al final...
	
	int valor=0;
	pos=0;
	while (valor == 0 && file_ext[pos]!=0 && ext[pos]!=0)
	{
		valor=file_ext[pos]-ext[pos];
		pos++;
	}
	
	if (valor==0){return 1;}
	return 0;
}

/*! \fn void str_ruta_back(char* ruta)
 *  \brief Borra el texto empezando desde el final hasta el primer '\' (El '\' se mantiene)
 *  \param ruta Ruta a la que se va a borrar el final
*/
void str_ruta_back(char* ruta)
{
	int pos=str_size(ruta);
	while (pos>=0 && (ruta[pos]!='\\' && ruta[pos]!='/')){pos--;}
	if(pos>0){ruta[pos+1]=0;}
}

/*! \fn void str_ruta_get_filename(char* ruta)
 *  \brief Borra todo lo que hay hasta el último '\', dejando así el nombre final y su extensión
 *	o el nombre de la carpeta final
 *  \param ruta Ruta a la que se va a borrar el final
*/
void str_ruta_get_filename(char* ruta)
{
	int pos=str_size(ruta);
	int extra=0;
	while (pos>=0 && (ruta[pos]!='\\' && ruta[pos]!='/')){pos--;}
	if(pos<=0){return;} // No hay una barra
	pos++; // pos apunta a la primera letra después del '/'
	
	while(ruta[pos+extra]!='\0')
	{
		ruta[extra]=ruta[pos+extra];
		extra++;
	}
	ruta[extra]=0;
}

/*! \fn void str_ext_back(char* ruta)
 *  \brief Borra el texto empezando desde el final hasta el primer '.' inclusive
 *  \param ruta Ruta a la que se va a borrar el final
*/
void str_ext_back(char* ruta)
{
	int pos=str_size(ruta);
	while (pos>=0 && ruta[pos]!='.'){pos--;}
	if(pos>0){ruta[pos]=0;}
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

float vdist(VECTOR vec1, VECTOR vec2)
{
	return sqrt( (vec1.x-vec2.x)*(vec1.x-vec2.x) + (vec1.y-vec2.y)*(vec1.y-vec2.y) + (vec1.z-vec2.z)*(vec1.z-vec2.z));
}

float vdist_sq(VECTOR vec1, VECTOR vec2)
{
	return ((vec1.x-vec2.x)*(vec1.x-vec2.x) + (vec1.y-vec2.y)*(vec1.y-vec2.y) + (vec1.z-vec2.z)*(vec1.z-vec2.z));
}

VECTOR v_from_ang(float pitch, float yaw)
{
	VECTOR vec;
	vec.x = -sin(yaw) * cos(pitch);
	vec.y = cos(yaw) * cos(pitch);
	vec.z = -sin(-pitch);
	return vec;
}

VECTOR vrotate(VECTOR coord, float pitch, float yaw, float roll)
{
	float sina, cosa, sinb, cosb, sing, cosg; // Para no tener que recalcularlos ya que se usan varias veces (Sin/cos de alfa, beta, gamma)
	VECTOR resultado;
	// Alfa = yaw, Beta = pitch, Gamma = roll
	
	sina = sin(yaw);
	cosa = cos(yaw);
	sinb = sin(pitch);
	cosb = cos(pitch);
	sing = sin(roll);
	cosg = cos(roll);
	
	resultado.x = coord.x * (cosa * cosb) + coord.y * (sina * cosb) + coord.z * (-sinb);
	resultado.y = coord.x * (cosa*sinb*sing - sina*cosg) + coord.y * (sina*sinb*sing + cosa*cosg) + coord.z * (cosb*sing);
	resultado.z = coord.x * (cosa*sinb*cosg + sina*sing) + coord.y * (sina*sinb*cosg - cosa*sing) + coord.z * (cosb*cosg);
	
	return resultado;
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
    time_t tim;
    time(&tim);
    file=fopen("debug.log","w");
    fprintf(file," --- DEBUG --- \n");
    fprintf(file," LOG: %s \n\n", ctime(&tim));
    fclose(file);
}

/*! \fn void debug_printf(const char *fmt, ...)
 *  \brief Escribe en el archivo debug.log como un fprintf
*/
void debug_printf(const char *fmt, ...)
{
    FILE* file;
    file=fopen("debug.log","a");
    va_list args;
    va_start(args, fmt);
    vfprintf (file, fmt, args);
    va_end(args);
    fclose(file);
}

/* - PRINTING - */

void set_gl_mode(void)
{
	/* Guardamos las matrices de proyección y de modelo */
	glMatrixMode (GL_MODELVIEW);
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,scr_width,scr_height,0);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_FOG);
	glDisable(GL_DEPTH_TEST);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	
	glColor3f(1.0f, 1.0f, 1.0f);
	
}

void restore_gl_mode(void)
{
	/* Cargamos las matrices de proyección y de modelo */
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_FOG);
	glEnable(GL_DEPTH_TEST);
	glColor3f(1.0f, 1.0f, 1.0f);
}

/*! \fn void position_printf(float x, float y, float z, const char *fmt, ...)
 *  \brief Muestra texto en el mundo en 3D al estilo de printf
 *  \param x,y,z Posición del texto en el mundo
 *  \param *fmt Texto de formato al estilo del printf
 *	Esta es una función muy básica ya que no permite girar ni cambiar el tamaño del texto.
*/
void position_printf(float x, float y, float z, const char *fmt, ...)
{
	SDL_Color clrFg = {255,255,255,0}; /* Color del texto */
	static char buf[256];
    int viewport[4];
    va_list args;

    va_start(args, fmt);
    (void) vsprintf (buf, fmt, args);
    va_end(args);
    
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	
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
        ////////glutStrokeString (font, (unsigned char *) buf);
		SDL_GL_RenderText(buf, fntCourier12, clrFg, x,y,z);
		//SDL_GL_SwapBuffers();
        //glColor3d(1.0,1.0,1.0);

    //glPopMatrix();
    //glMatrixMode(GL_MODELVIEW);
    //glPopMatrix();
}

/*! \fn void hud_printf(float x, float y, const char *fmt, ...)
 *  \brief Muestra texto en el HUD (2D) al estilo de printf
 *  \param x,y Coordenadas en la que mostrar el texto
 *  \param *fmt Texto de formato al estilo del printf
 *	Esta es una función muy básica ya que no permite cambiar el color del texto.
*/
void hud_printf(float x, float y, const char *fmt, ...)
{
	SDL_Color clrFg = {255,255,255,128}; /* Color del texto */
	glDisable(GL_LIGHTING);
	glDisable(GL_FOG);
    glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	static char buf[256];
    va_list args;

    va_start(args, fmt);
	(void) vsprintf (buf, fmt, args);
    va_end(args);
    
	SDL_GL_RenderText(buf, fntCourier12, clrFg, x, y, 0.1f);
	
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
	glColor3f(1.0f,1.0f,1.0f);
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
	glColor3f(1.0f,1.0f,1.0f);
	/*glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x-size*(sin(RAD(camera.yaw+90))), y+size*(cos(RAD(camera.yaw+90))-cos(RAD(camera.pitch))), z-size*sin(RAD(camera.pitch)));// Bottom Left
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+size*(sin(RAD(camera.yaw+90))), y-size*(cos(RAD(camera.yaw+90))+cos(RAD(camera.pitch))), z-size*sin(RAD(camera.pitch)));// Bottom Right
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+size*(sin(RAD(camera.yaw+90))), y-size*(cos(RAD(camera.yaw+90))-cos(RAD(camera.pitch))), z+size*sin(RAD(camera.pitch)));// Top Right
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x-size*(sin(RAD(camera.yaw+90))), y+size*(cos(RAD(camera.yaw+90))+cos(RAD(camera.pitch))), z+size*sin(RAD(camera.pitch)));// Top Left
	glEnd();
	*/
	VECTOR vect = {x,y,z};
	normalize(&vect);
	VECTOR vup = vrotate(vect, RAD(90), 0, 0);
	VECTOR vright = vrotate(vect, 0, RAD(180), 0);
	
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+size*(-vup.x-vright.x), y+size*(-vup.y-vright.y), z+size*(-vup.z-vright.z));// Bottom Left
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+size*(-vup.x+vright.x), y+size*(-vup.y+vright.y), z+size*(-vup.z+vright.z));// Bottom Right
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+size*(vup.x+vright.x), y+size*(vup.y+vright.y), z+size*(vup.z+vright.z));// Top Right
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+size*(vup.x-vright.x), y+size*(vup.y-vright.y), z+size*(vup.z-vright.z));// Top Left
	glEnd();
}


/* RANDOM */
/* rand() % n da resultados uniformes de [0 - (n-1)] */
void randomize(float seed){srand(time(NULL)*seed);}
int irand(int max){return (rand() % (max-1));}
int sirand(int max){return (rand() % (max-1)*2)-max;}
float frand(void){return (rand() % 10001)/10000.0f;}
float sfrand(void){return ((rand() % 20001)-10000)/10000.0f;}
