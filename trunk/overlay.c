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
	int w,h;
	if (!minimap)return;
	set_gl_mode();
	
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, minimap);
	
	//glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
	//glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
	w=1024;
	h=799;
	
	glBegin(GL_QUADS);
		glTexCoord2f(1,1);
		glVertex2i(scr_width-2, 2);
		glTexCoord2f(0,1);
		glVertex2i(scr_width-w/4-2, 2);
		glTexCoord2f(0,0);
		glVertex2i(scr_width-w/4-2, h/4+2);
		glTexCoord2f(1,0);
		glVertex2i(scr_width-2, h/4+2);
	glEnd();
	
	float x, y;
	x = (((camera.pos_x+marte.ini_x)/marte.scale) + marte.tam_x/2)/marte.tam_x  *w/4 +scr_width-w/4-2;
	y = (-((camera.pos_y+marte.ini_y)/marte.scale) + marte.tam_y/2)/marte.tam_y  *h/4 +2;
	
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.0f, 0.0f, 1.0f);
	glPointSize(2.0);
	glBegin(GL_POINTS);
		glVertex2f(x,y);
	glEnd();
	
	glBegin(GL_TRIANGLES);
		glColor4f(0.0f, 0.0f, 1.0f, 0.6f);
		glVertex2f(x,y);
		glColor4f(0.0f, 0.0f, 1.0f, 0.0f);
		glVertex2f(x+sin(RAD(-camera.yaw-22.5))*20 +cos(RAD(-camera.yaw))*20,
					+y-cos(RAD(camera.yaw+22.5))*20 +sin(RAD(-camera.yaw))*20);
		glVertex2f(x+sin(RAD(-camera.yaw+22.5))*20 -cos(RAD(-camera.yaw))*20,
					+y-cos(RAD(camera.yaw-22.5))*20 -sin(RAD(-camera.yaw))*20);
	glEnd();
	
	restore_gl_mode();
}

void draw_HUD(void)
{
/* DIALOG: {(*df), x, y, w, h, fg, bg, key, flag, d1, d2, *dp, *dp2, *dp3} */
	DIALOG box1 = {d_box_proc, 0, 0, 430, 150 ,{0,255,0,128} ,{0,128,0,128}, 0, 0, 0, 0, NULL, NULL, NULL};
	/*DIALOG lbl_test[] = {
	{d_label_proc, 20, 220, 80, 12 ,{255,0,0,255} ,{0,128,0,128}, 0, 0, 0, 0, "Hola mundo1!", fntArial12, NULL},
	{d_label_proc, 20, 232, 80, 12 ,{255,0,0,255} ,{0,128,0,128}, 0, 0, 0, 0, "Hola mundo2!", fntArial12, NULL},
	{d_label_proc, 20, 244, 80, 12 ,{255,0,0,255} ,{0,128,0,128}, 0, 0, 0, 0, "Hola mundo3!", fntArial12, NULL},
	{d_label_proc, 20, 256, 80, 12 ,{255,0,0,255} ,{0,128,0,128}, 0, 0, 0, 0, "Hola mundo4!", fntArial12, NULL},
	{NULL_DIALOG}};
	
	draw_dialog(lbl_test);*/
	draw_element(box1);
	
	draw_minimap(minimapa);
	
	hud_printf (12, 2*12, "Mars Base - v" VER_STRING);
	hud_printf (12, 3*12, "PITCH, YAW, ROLL = (%.2f, %.2f, %.2f)",camera.pitch, camera.yaw, camera.roll);
	hud_printf (12, 4*12, "POS = (%.2f, %.2f, %.2f)",camera.pos_x, camera.pos_y, camera.pos_z);
	hud_printf (12, 5*12, "Flechas para moverse, shift para ir muy rápido");
	hud_printf (12, 6*12, "WASD+QE -> Girar cámara");
	hud_printf (12, 7*12, "IJKL -> Girar objeto");
	hud_printf (12, 8*12, "C/V -> Ver/ocultar la cuadrícula");
	hud_printf (12, 9*12, "B/N -> Ver/ocultar las normales");
}

/*! \fn int draw_dialog(DIALOG d[])
 *  \brief Dibuja todos los elementos del dialog. NOTA: No inicializa el dialog, solo los dibuja
 *  \param d[] Matriz de dialogs, el último elemento debe ser NULL_DIALOG
 *  \return D_O_K Si todo fué bien
*/

int draw_dialog(DIALOG d[])
{
	int res=D_O_K;
	int ret;
	int cont=0;
	set_gl_mode();
	while(d[cont].df!=NULL)
	{
		ret=d[cont].df(MSG_DRAW,&d[cont],0);
		if(ret!=D_O_K){res=ret;}
		cont++;
	}
	restore_gl_mode();
	return res;
}

/*! \fn int draw_element(DIALOG d)
 *  \brief Envía un mensaje de dibujo al dialog. NOTA: No inicializa el elemento
 *  \param d Dialog que se debe dibujar
 *  \return D_O_K Si todo fué bien
*/
int draw_element(DIALOG d)
{
	int res;
	set_gl_mode();
	res=d.df(MSG_DRAW,&d,0);
	restore_gl_mode();
	return res;
}


int d_box_proc(int msg, struct st_Dialog * d, int c)
{
	switch (msg)
	{
		case MSG_START:
			
			break;
		case MSG_END:
			
			break;
		case MSG_DRAW:
			glColor4f(d->bg.r/255.0f, d->bg.g/255.0f, d->bg.b/255.0f, d->bg.a/255.0f);
			glBegin(GL_QUADS);
				glVertex2i(d->x, d->y);				/* Arriba izquierda */
				glVertex2i(d->x, d->y+d->h);		/* Abajo izquierda */
				glVertex2i(d->x+d->w, d->y+d->h);	/* Abajo derecha */
				glVertex2i(d->x+d->w, d->y);		/* Arriba derecha */
			glEnd();
			glLineWidth(2);
			glColor4f(d->fg.r/255.0f, d->fg.g/255.0f, d->fg.b/255.0f, d->fg.a/255.0f);
			glBegin(GL_LINE_LOOP);
				glVertex2i(d->x+1, d->y+1);				/* Arriba izquierda */
				glVertex2i(d->x+1, d->y+d->h-1);		/* Abajo izquierda */
				glVertex2i(d->x+d->w-1, d->y+d->h-1);	/* Abajo derecha */
				glVertex2i(d->x+d->w-1, d->y+1);		/* Arriba derecha */
			glEnd();
			break;
		case MSG_IDLE:
			
			break;
	}
	return D_O_K;
}

int d_line_proc(int msg, struct st_Dialog * d, int c)
{
	switch (msg)
	{
		case MSG_START:
			
			break;
		case MSG_END:
			
			break;
		case MSG_DRAW:
			if (d->d1>0){glLineWidth(d->d1);}else{glLineWidth(1);}
			glColor4f(d->fg.r/255.0f, d->fg.g/255.0f, d->fg.b/255.0f, d->fg.a/255.0f);
			glBegin(GL_LINE);
				glVertex2i(d->x, d->y);
				glVertex2i(d->w, d->h);
			glEnd();
			break;
		case MSG_IDLE:
			
			break;
	}
	return D_O_K;
}

int d_label_proc(int msg, struct st_Dialog * d, int c)
{
	switch (msg)
	{
		case MSG_START:
			
			break;
		case MSG_END:
			
			break;
		case MSG_DRAW:
			glColor4f(d->bg.r/255.0f, d->bg.g/255.0f, d->bg.b/255.0f, d->bg.a/255.0f);
			SDL_Color clrFg = {d->fg.r, d->fg.g, d->fg.b, d->fg.a}; /* Color del texto */
			
			glBegin(GL_QUADS);
				glVertex2i(d->x, d->y-d->h);		/* Arriba izquierda */
				glVertex2i(d->x, d->y);				/* Abajo izquierda */
				glVertex2i(d->x+d->w, d->y);		/* Abajo derecha */
				glVertex2i(d->x+d->w, d->y-d->h);	/* Arriba derecha */
			glEnd();
			
			SDL_GL_RenderText(d->dp, (TTF_Font*)d->dp2, clrFg, d->x, d->y, 0.1);
			
			break;
		case MSG_IDLE:
			
			break;
	}
	return D_O_K;
}
