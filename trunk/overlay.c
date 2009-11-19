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

/** \file overlay.c
 * \brief Controla el HUD
 * \author Alfonso Arbona Gimeno
*/

#include "overlay.h"
#include "heightmap.h"

void draw_minimap(GLuint minimap)
{
	if (!minimap)return;
	
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glFrustum(0,1,0,1,0.001,-2);
	
	glTranslatef(0.5f,0.5f,-2.0f);
	
	glDisable(GL_LIGHTING);
	glDisable(GL_FOG);
	glDisable(GL_DEPTH_TEST);
	
	
	
	glBindTexture(GL_TEXTURE_2D, minimap);
	glBegin(GL_QUADS);
		glTexCoord2f(1,1);
		glVertex2f(+0.5f,  +0.3f);
		glTexCoord2f(0,1);
		glVertex2f(+0.0f,  +0.3f);
		glTexCoord2f(0,0);
		glVertex2f(+0.0f,  +0.0f);
		glTexCoord2f(1,0);
		glVertex2f(+0.5f,  +0.0f);
	glEnd();
	
	/*
	y
	^(0, 0.3)
	|
	|      (0.5, 0)
	o-----> x
	*/
	
	float x, y;
	x = ((camera.pos_x/marte.scale) + marte.tam_x/2 - 1)*0.5f/ marte.tam_x;
	y = ((camera.pos_y/marte.scale) + marte.tam_y/2 - 1)*0.3f/ marte.tam_y;
	
	glDisable(GL_TEXTURE_2D);
	glColor3d(0.0f, 0.0f, 1.0f);
	glPointSize(4.0);
	glBegin(GL_POINTS);
		glVertex2f(x,y);
	glEnd();
	glBegin(GL_LINES);
		glVertex2f(x,y);
		glVertex2f(x+sin(RAD(-camera.yaw))*0.02f,y+cos(RAD(camera.yaw))*0.02f);
	glEnd();
	
	glPopMatrix();
	
	
	
	/* Mostramos el texto de información en el HUD */
	hud_printf (0, 0, "Mars Base - v" VER_STRING);
	hud_printf (0, 1, "PITCH, YAW, ROLL = (%.2f, %.2f, %.2f)",camera.pitch, camera.yaw, camera.roll);
	hud_printf (0, 2, "POS = (%.2f, %.2f, %.2f)",camera.pos_x, camera.pos_y, camera.pos_z);
	hud_printf (0, 3, "Flechas para moverse, shift para ir muy rápido");
	hud_printf (0, 4, "WASD+QE -> Girar cámara");
	hud_printf (0, 5, "IJKL -> Girar objeto");
	hud_printf (0, 6, "C/V -> Ver/ocultar la cuadrícula");
	hud_printf (0, 7, "B/N -> Ver/ocultar las normales");
	
	glEnable(GL_FOG);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

void draw_HUD(void)
{
	draw_minimap(minimapa);
	
}
