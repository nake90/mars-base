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

#include "overlay.h"

void draw_minimap(GLuint minimap)
{
	if (!minimap)return;
	
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glFrustum(0,1,0,1,0.001,-2);
	
	glTranslatef(0,0,-2);
	
	glDisable(GL_LIGHTING);
	glDisable(GL_FOG);
	glDisable(GL_DEPTH_TEST);
	
	
	
	glBindTexture(GL_TEXTURE_2D, minimap);
	glBegin(GL_QUADS);
		glTexCoord2f(1,1);
		glVertex2f(+1.0f,  +0.8f);
		glTexCoord2f(0,1);
		glVertex2f(+0.5f,  +0.8f);
		glTexCoord2f(0,0);
		glVertex2f(+0.5f,  +0.5f);
		glTexCoord2f(1,0);
		glVertex2f(+1.0f,  +0.5f);
	glEnd();
	/* Falta que muestre la posición actual */
	
	glEnable(GL_FOG);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glPopMatrix();
}

void draw_HUD(void)
{
	draw_minimap(minimapa);
	//glutSwapBuffers();
}
