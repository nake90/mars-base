#ifndef SHARED_H
#define SHARED_H

#include "GL/openglut.h"
#include "IL/ilut.h"

#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>

//2*PI/360 = 0.017453292519943295769236907684886
#define RAD(x) (float)x*0.01745329


/* Realmente el tamaño es el doble de este valor */
#define TERR_SIZE 100
int tam_mapa_x;
int tam_mapa_y;

typedef struct{float x; float y; float z;}VECTOR;

typedef struct
{
	GLfloat pos_x;
	GLfloat pos_y;
	GLfloat pos_z;
	
	GLfloat pitch;
	GLfloat yaw;
	GLfloat roll;
}t_camera;
t_camera camera;

typedef struct
{
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess[1];
	GLint texture[1];
}t_texture;

void debug_reset (void);
void debug_printf (const char *fmt, ...);
void position_printf (float x, float y, float z, const char *fmt, ...);
void hud_printf (int col, int row, const char *fmt, ...);
void use_texture (t_texture texture);
//void load_material (t_texture texture);



#endif
