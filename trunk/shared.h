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

/** \file shared.h
 * \brief Definiciones compartidas por todos los m�dulos
 * Este archivo se encarga de todas las definiciones, structs, funciones, ... que son necesarias por todos los
 * diferentes m�dulos del programa. Est� formado por funciones de bajo nivel personalizadas para la aplicaci�n.
 * \author Alfonso Arbona Gimeno
*/

#ifndef SHARED_H
#define SHARED_H

#if defined (__WIN32__)
#	define WINDOWS
#else
#	define LINUX
#endif

#include <GL/gl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include <stdarg.h>
#include "mars_base_private.h"
#include "lang.h"

//2*PI/360 = 0.017453292519943295769236907684886
#define RAD(x) (float)((x)*0.01745329)

#define TICK_INTERVAL 20
/* FPS = 1000/TICK_INTERVAL => TICK_INTERVAL=1000/FPS */


#define MAX_SCREEN_MESSAGES 80
#define MAX_SCREEN_MESSAGES_LEN 256
char scr_messages[MAX_SCREEN_MESSAGES][MAX_SCREEN_MESSAGES_LEN];
int scr_message_index;
int scr_message_debug;

//char app_path[256];

extern GLuint minimapa;


extern TTF_Font *fntCourier12;
extern TTF_Font *fntArial12;


typedef struct{float x; float y; float z;}VECTOR;
typedef struct{float r; float g; float b; float a;}COLORf;


#define CAMERA_MIN_VEL 0.001
#define CAMERA_VEL_DIV 1.1
typedef struct
{
	GLfloat pos_x;
	GLfloat pos_y;
	GLfloat pos_z;
	
	GLfloat vel_x;
	GLfloat vel_y;
	GLfloat vel_z;
	
	GLint wasd_count;/* Cuenta el n�mero de teclas de movimiento presionadas. (Para la desaceleraci�n) */
	
	GLfloat pitch;
	GLfloat yaw;
	GLfloat roll;
	
	int show_grid;
	int show_presion;
	
	int ghost_mode;
}t_camera;
extern t_camera camera;

typedef struct
{
	int show_fondo;
	
	int show_grid;
	int show_presion;
}t_config;
extern t_config config;

typedef struct
{
	char name[80];
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess[1];
	GLfloat transparency[1];
	GLint texture[1];
}t_texture;

/* DEBUG y SCREEN */
void debug_reset (void);
void debug_printf (const char *fmt, ...);
void scr_init_reset (int debug_too);
void scr_init_printf (const char *fmt, ...);
void scr_init_reprintf (const char *fmt, ...);/* Como scr_init_printf pero sobre la �ltima l�nea */
void position_printf (float x, float y, float z, const char *fmt, ...);
void hud_printf (float x, float y, const char *fmt, ...);
void set_gl_mode(void);
void restore_gl_mode(void);

/* TEXTURAS */
void use_texture (t_texture texture);

/* STRINGS */
int str_size(const char* string);
#define str_len str_size
void str_cpy(char* string1,const char* string2);
void str_cpyl(char* string1, int len, const char* string2);
void str_append(char* string1,const char* string2);
int str_cmp(const char* string1,const char* string2);
int str_ext_cmp(const char* ruta, const char* ext); // Compara la extensi�n (0 si son distintos, 1 si son iguales)
void str_ruta_back(char* ruta);
void str_ext_back(char* ruta); // Quita la extensi�n al nombre del archivo
void str_ruta_get_filename(char* ruta); // Obtiene el nombre y su extensi�n
int str_list_find(const char* find,const char* table[], int elementos);
#define __STR2FLOAT_DEFINED
float str2float(const char *str);

/* VECTORES */
VECTOR p_vect(VECTOR vec1,VECTOR vec2);
float p_escalar(VECTOR vec1,VECTOR vec2);
void normalize(VECTOR* vec1);
VECTOR vadd(VECTOR vec1, VECTOR vec2);
VECTOR vsub(VECTOR vec1, VECTOR vec2);
float vdist(VECTOR vec1, VECTOR vec2); /* Distancia entre ambos vectores */
float vdist_sq(VECTOR vec1, VECTOR vec2); /* Cuadrado de la distancia */
VECTOR v_from_ang(float pitch, float yaw); /* Vector definido por pitch y yaw en radianes */
VECTOR vrotate(VECTOR coord, float pitch, float yaw, float roll); /* Rotar el vector */

/* MATEM�TICAS */
float nabs(float val);
int nround(float val);
int nfloor(float val);
int nceil(float val);
int nsgn(float val);

/* RANDOM */
void randomize(float seed);
int irand(int max); // Integer random [0 - max]
int sirand(int max); // Signed integer random [(-max) - (+max)]
float frand(void); // Float random [0 - 1]
float sfrand(void); // Signed float random [(-1) - (+1)]
#define rfrand(M,m) (frand()*((M)-(m))+(m))

/* SPRITES */
void draw_fixsprite (float x, float y, float z, t_texture textura, float size);
void draw_sprite (float x, float y, float z, t_texture textura, float size);

/* TEXTO EN EL MUNDO */
void SDL_GL_RenderText(const char *text, TTF_Font *font, SDL_Color color, float x, float y, float z);

/* OPENGL */
int isExtensionSupported(const char *extension);

/* VARIABLES */
extern int scr_width, scr_height, scr_bpp; // Es usado en muchos sitios que no necesitan incluir display.h
extern t_texture fondo;
extern int null_texture;  // Textura usada si hay alg�n error

#endif
