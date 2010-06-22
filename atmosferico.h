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

/** \file atmosferico.h
 * \brief Header sobre todos los fenómenos atmosféricos y físicos del exterior
 * \author Alfonso Arbona Gimeno
*/

#ifndef ATMOSFERICO_H
#define ATMOSFERICO_H

#include "shared.h"
/* Dynamic Pressure: q(Pa) = 1/2 * densidad(kg/m3) * v^2(m/s) */

#define SUN_SIZE 4000.0f

/* Presión media en Pascales */
#define PRESION_MEDIA 800
#define PRESION_MAX  1000
#define PRESION_MIN   600

#define TEMPERATURA_MEDIA 218
#define TEMPERATURA_MAX   300
#define TEMPERATURA_MIN   140

#define Pa2Atm(x) (x)*9.86923e-6
#define Atm2Pa(x) (x)/9.86923e-6

#define GAS_R 0.08205746 // (Atm * l) / (mol * K)
#define GAS_Na 6.02214179E23 // Número de Avogadro (Espero no usarlo porque mira que es grande...)

//#define SKYBOX_DISTANCE   55000 /* 50000 es el máximo de niebla... */
//GLint skybox[5]; /*!< Texturas del skybox. 0->Front; 1->Right; 2-> Back; 3-> Left; 4->Top*/

/* Niebla y atmósfera */
extern float fogColor[4];/*= {0.81f, 0.64f, 0.61f, 1.0f}; !< Color de la niebla */
extern float fogRange[2];/*= {25000.0f, 50000.0f}; !< Distancia mínima de la niebla y distancia máxima visible en metros */

typedef struct
{
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float position[3];
	float hora[1];
	t_texture material;
}t_sun;

// Masas atómicas de las moléculas
extern const double ma_co2;
extern const double ma_n2;
extern const double ma_ar;
extern const double ma_o2;
extern const double ma_co;
extern const double ma_h20;

// Viscosidades de los gases
extern const double visc_co2;
extern const double visc_n2;
extern const double visc_ar;
extern const double visc_o2;
extern const double visc_co;
extern const double visc_h20;
extern const double visc_media;


// Densidades de las moléculas
extern const double densN_co2;
extern const double densN_n2;
extern const double densN_ar;
extern const double densN_o2;
extern const double densN_co;
extern const double densN_h20;

typedef struct
{
	float CO2;
	float N2;
	float Ar;
	float O2;
	float CO;
	float H2O;
}t_gas;	/* Lista de cantidad de moles de cada tipo de gas (PV=nRT) */


typedef struct
{
	t_gas gases;	// Moles en un 1m3 de aire
	float presion; // Se actualiza cada frame? depende de temperatura y gases
	float temperatura;
}t_atmosfera;

typedef struct
{
	t_gas gases;
	float volumen;
	float temperatura;
}t_node_data;

extern t_atmosfera datos_atmosfera;

extern t_sun sun;
//extern t_texture sun_texture;

float get_density(float normal_density, float temperatura, float presion);
float get_presion(t_gas gases, float volumen, float temperatura);
void node_flow_gas(t_node_data *node1, t_node_data *node2, float size, float distancia, float step_time);
float moles_gas_total(t_gas gas);

void node_main_control(float step_time);
#endif
