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
	int texture[2]; /* Textura 1 es la imágen, textura 2 es el mask */
}t_sun;

// Masas atómicas de los elementos (Definidos en el .c)
extern const double ma_co2;
extern const double ma_ar;
extern const double ma_o2;
extern const double ma_co;
extern const double ma_h20;
extern const double ma_h2;

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

extern t_atmosfera datos_atmosfera;

extern t_sun sun;
extern t_texture sun_texture;

float get_presion(t_gas gases, float volumen, float temperatura);

#endif
