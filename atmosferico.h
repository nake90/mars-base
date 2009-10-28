#ifndef ATMOSFERICO_H
#define ATMOSFERICO_H

#include <GL/openglut.h>
#include <IL/ilut.h>

#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>

#include "shared.h"
/* Dynamic Pressure: q(Pa) = 1/2 * densidad(kg/m3) * v^2(m/s) */

/* Número de refrescos de pantalla antes de recalcular la atmósfera */
#define ATM_UPDATE 10

/* Presión media en Pascales */
#define PRESION_MEDIA 800
#define PRESION_MAX  1000
#define PRESION_MIN   600
#define CICLONES              10
#define CICLONES_VAL_MAX       2
#define CICLONES_DELTA_ACEL    5
#define CICLONES_MAX_VEL       5
#define CICLONES_MIN_DIST     50

typedef struct
{
	float pos_x;
	float pos_y;
	float vel_x;
	float vel_y;
	float acel_x;
	float acel_y;
	float valor;
}t_ciclon; /* Valores negativos -> Bajas presiones; Valores positivos -> Altas presiones */

typedef struct
{
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat position[4];
	float hora[1];
}t_sun;

t_ciclon ciclon[CICLONES];


float presion[TERR_SIZE*2 * TERR_SIZE*2];
float presion_cicl[TERR_SIZE*2 * TERR_SIZE*2];
float presion_m;
float presion_m_c;


float get_presion (int x, int y);
void set_presion (float val, int x, int y);
float get_presion_cicl (int x, int y);
void set_presion_cicl (float val, int x, int y);
void set_presion_from_cicl (float val, int x, int y);
void randomize_presion (float media, float rango);
void calc_presion_cicl (void);
void add_noise_presion (float rango);
void flow_presion (float factor);
void init_ciclon (void);
void update_ciclon (void);
double get_ciclon_adder (int i, int x, int y);

void update_sun_now (t_sun sol);


#endif
