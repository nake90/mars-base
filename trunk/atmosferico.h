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

#include "GL/openglut.h"
#include "IL/ilut.h"
#include "heightmap.h"

#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>

#include "shared.h"
/* Dynamic Pressure: q(Pa) = 1/2 * densidad(kg/m3) * v^2(m/s) */

#define SUN_SIZE 500.0f

/* Presión media en Pascales */
#define PRESION_MEDIA 800
#define PRESION_MAX  1000
#define PRESION_MIN   600

typedef struct
{
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat position[3];
	float hora[1];
	GLint texture[2]; /* Textura 1 es la imágen, textura 2 es el mask */
}t_sun;

#endif
