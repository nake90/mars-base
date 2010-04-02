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

/** \file shared.h
 * \brief Definiciones compartidas por todos los módulos
 * Este archivo se encarga de todas las definiciones, structs, funciones, ... que son necesarias por todos los
 * diferentes módulos del programa. Está formado por funciones de bajo nivel personalizadas para la aplicación.
 * \author Alfonso Arbona Gimeno
*/

#ifndef SHARED_H
#define SHARED_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <IL/ilut.h>

#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>
#include "mars_base_private.h"

//2*PI/360 = 0.017453292519943295769236907684886
#define RAD(x) (float)((x)*0.01745329)

#define TICK_INTERVAL 20
/* FPS = 1000/TICK_INTERVAL => TICK_INTERVAL=1000/FPS

Realmente el tamaño es el doble de este valor #OBSOLETO... sigue estando ahí porque aún no me he encargado de atmosferico.h */
#define TERR_SIZE 100

int tam_mapa_x;
int tam_mapa_y;

#define MAX_SCREEN_MESSAGES 80
#define MAX_SCREEN_MESSAGES_LEN 256
char scr_messages[80][255];
int scr_message_index;
int scr_message_debug;

char app_path[256];

GLuint minimapa;

TTF_Font *fntCourier12;
TTF_Font *fntArial12;


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
	
	GLint wasd_count;/* Cuenta el número de teclas de movimiento presionadas. (Para la desaceleración) */
	
	GLfloat pitch;
	GLfloat yaw;
	GLfloat roll;
	
	int show_grid;
	int show_presion;
}t_camera;
t_camera camera;

typedef struct
{
	int show_fondo;
	
	int show_grid;
	int show_presion;
}t_config;
t_config config;

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

void debug_reset (void);
void debug_printf (const char *fmt, ...);
void scr_init_reset (int debug_too);
void scr_init_printf (const char *fmt, ...);
void scr_init_reprintf (const char *fmt, ...);/* Como scr_init_printf pero sobre la última línea */
void position_printf (float x, float y, float z, const char *fmt, ...);
void hud_printf (float x, float y, const char *fmt, ...);
void set_gl_mode(void);
void restore_gl_mode(void);

void use_texture (t_texture texture);

/* Trace *//*
VECTOR trace_camera(float x, float y, float z)
{
	VECTOR vec={0,0,0};
	float d;
	if (camera.pitch>=80 || camera.pitch<=-80){return vec;}
	d=tan(RAD(camera.pitch))*z;
	vec.y=d*cos(RAD(-camera.yaw))+y;
	vec.x=d*sin(RAD(-camera.yaw))+x;
	return vec;
}*/

int str_size(const char* string);
#define str_len str_size
void str_cpy(char* string1,const char* string2);
void str_cpyl(char* string1, int len, const char* string2);
void str_append(char* string1,const char* string2);
int str_cmp(const char* string1,const char* string2);
void str_ruta_back(char* ruta);
int str_list_find(const char* find,const char* table[], int elementos);

VECTOR p_vect(VECTOR vec1,VECTOR vec2);
float p_escalar(VECTOR vec1,VECTOR vec2);
void normalize(VECTOR* vec1);
VECTOR vadd(VECTOR vec1, VECTOR vec2);
VECTOR vsub(VECTOR vec1, VECTOR vec2);
float vdist(VECTOR vec1, VECTOR vec2); /* Distancia entre ambos vectores */
float vdist_sq(VECTOR vec1, VECTOR vec2); /* Cuadrado de la distancia */
VECTOR v_from_ang(float pitch, float yaw); /* Vector definido por pitch y yaw en radianes */

float nabs(float val);
int nround(float val);
int nfloor(float val);
int nceil(float val);
int nsgn(float val);

void draw_fixsprite (float x, float y, float z, t_texture textura, float size);
void draw_sprite (float x, float y, float z, t_texture textura, float size);

void SDL_GL_RenderText(char *text, TTF_Font *font, SDL_Color color, float x, float y, float z);

extern int scr_width, scr_height, scr_bpp;
extern t_texture fondo;

#endif
