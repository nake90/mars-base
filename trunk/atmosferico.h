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

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <SDL_ttf.h>
#include <IL/ilut.h>
#include "heightmap.h"

#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>

#include "shared.h"
/* Dynamic Pressure: q(Pa) = 1/2 * densidad(kg/m3) * v^2(m/s) */

#define SUN_SIZE 2000.0f

/* Presión media en Pascales */
#define PRESION_MEDIA 800
#define PRESION_MAX  1000
#define PRESION_MIN   600

//#define SKYBOX_DISTANCE   55000 /* 50000 es el máximo de niebla... */
//GLint skybox[5]; /*!< Texturas del skybox. 0->Front; 1->Right; 2-> Back; 3-> Left; 4->Top*/

/* Niebla y atmósfera */
extern GLfloat fogColor[4];/*= {0.81f, 0.64f, 0.61f, 1.0f}; /*!< Color de la niebla */
extern GLfloat fogRange[2];/*= {25000.0f, 50000.0f}; /*!< Distancia mínima de la niebla y distancia máxima visible en metros */

typedef struct
{
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat position[3];
	float hora[1];
	GLint texture[2]; /* Textura 1 es la imágen, textura 2 es el mask */
}t_sun;

/* TEXTURAS BÁSICAS */
/*					AMBIENT					DIFFUSE					SPECULAR				POSITION			HORA   TEXTURES*/
extern t_sun sun;/*={{0.5f, 0.5f, 0.5f, 1.0f},{1.0f, 1.0f, 1.0f, 1.0f},{1.0f, 1.0f, 1.0f, 1.0f},{10000.0f, 20000.0f, 30000.0f},{12.0f},{0,0}};
/*							AMBIENT						DIFFUSE						SPECULAR		SHININESS TEXTURE */
extern t_texture sun_texture;/*={{1.0f, 1.0f, 1.0f, 1.0f},{1.0f, 1.0f, 1.0f, 1.0f},{0.0f, 0.0f, 0.0f, 1.0f},{1.0},{0}};*/

#endif
