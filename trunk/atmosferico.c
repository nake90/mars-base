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

/** \file atmosferico.c
 * \brief Controla todos los fenómenos atmosféricos y físicos del exterior
 * \author Alfonso Arbona Gimeno
*/

#include "atmosferico.h"

/* Niebla y atmósfera */
GLfloat fogColor[4]= {0.81f, 0.64f, 0.61f, 1.0f}; /*!< Color de la niebla */
GLfloat fogRange[2]= {25000.0f, 50000.0f}; /*!< Distancia mínima de la niebla y distancia máxima visible en metros */

/* TEXTURAS BÁSICAS */
/*					AMBIENT					DIFFUSE					SPECULAR				POSITION			HORA   TEXTURES*/
t_sun sun={{0.5f, 0.5f, 0.5f, 1.0f},{1.0f, 1.0f, 1.0f, 1.0f},{1.0f, 1.0f, 1.0f, 1.0f},{10000.0f, 20000.0f, 30000.0f},{12.0f},{0,0}};
/*							AMBIENT						DIFFUSE						SPECULAR		SHININESS TEXTURE */
t_texture sun_texture=	{{1.0f, 1.0f, 1.0f, 1.0f},{1.0f, 1.0f, 1.0f, 1.0f},{0.0f, 0.0f, 0.0f, 1.0f},{1.0},{0}};

// Masas atómicas de los elementos
const double ma_co2=12.011 + 15.999 * 2;
const double ma_ar=39.948;
const double ma_o2=15.999 * 2;
const double ma_co=12.011 + 15.999;
const double ma_h20=1.0079 * 2 + 15.999;
const double ma_h2=1.0079 * 2;

/* Datos iniciales de la atmósfera */
t_atmosfera datos_atmosfera =
{
/* Gases */			{
/* CO2 */			0.9572 * PRESION_MEDIA / (GAS_R * TEMPERATURA_MEDIA),
/* N2 */			0.0270 * PRESION_MEDIA / (GAS_R * TEMPERATURA_MEDIA),
/* Ar */			0.0160 * PRESION_MEDIA / (GAS_R * TEMPERATURA_MEDIA),
/* O2 */			0.0013 * PRESION_MEDIA / (GAS_R * TEMPERATURA_MEDIA),
/* CO */			0.0007 * PRESION_MEDIA / (GAS_R * TEMPERATURA_MEDIA),
/* H2O */			0.0003 * PRESION_MEDIA / (GAS_R * TEMPERATURA_MEDIA),
					},
/* Presión */		PRESION_MEDIA,
/* Temperatura */	TEMPERATURA_MEDIA
};

float get_presion(t_gas gases, float volumen, float temperatura)
{
	if(volumen <= 0 || temperatura <= 0)return 0;
	float presion;
	int i;
	// Presión total = Sum(Presiones parciales) y Presión parcial = n * R * T / Vtot
	presion = (gases.CO2 * GAS_R * temperatura) / volumen;
	presion += (gases.N2 * GAS_R * temperatura) / volumen;
	presion += (gases.Ar * GAS_R * temperatura) / volumen;
	presion += (gases.O2 * GAS_R * temperatura) / volumen;
	presion += (gases.CO * GAS_R * temperatura) / volumen;
	presion += (gases.H2O * GAS_R * temperatura) / volumen;
	
	return presion;
}
