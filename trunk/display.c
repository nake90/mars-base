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

/** \file display.h
 * \brief Funciones y definiciones de control de salida por pantalla
 * Este archivo se encarga de todas las definiciones, structs, funciones, ... que son necesarias para
 * mostrar datos por pantalla. Contiene funciones de iniciaci�n, de mantenimiento, de control, de
 * display, de limpieza de pantalla.
 * \author Alfonso Arbona Gimeno
*/
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <SDL/SDL_ttf.h>
#include <IL/ilut.h>
//#include <stdio.h>
//#include <math.h>
//#include <time.h>
#include "atmosferico.h"
#include "objetos.h"
#include "display.h"
#include "overlay.h"

int scr_width = -1;
int scr_height = -1;
int scr_bpp;
int scr_flags = -1;

void message_printf(const char *fmt, ...)
{
	char buf[1024];
	char line[1024];
    va_list args;

    va_start(args, fmt);
	(void) vsprintf (buf, fmt, args);
    va_end(args);
    
    if(str_size(buf)<=0)return;
    
    int lines=1;
    int i = 0, cont=0;;
    int max_len_size=-1;
    int l_mas_larga =-1;
    
    while(buf[i]!='\0')
	{
		if(buf[i]=='\n')
		{
			if(max_len_size<cont){max_len_size=cont; l_mas_larga=lines-1;}
			cont=-1;
			lines++;
		}
		cont++;
		i++;
	}
	if(max_len_size<cont){max_len_size=cont; l_mas_larga=lines-1;} // Si no est� esto la �ltima l�nea no se tiene en cuenta para el tama�o
	
	int width, height, box_h;
	
	cont=0;
	int j;
	for(i=0; i<lines; i++)
	{
		// Usamos el buffer 'lista_texto' empezando por TEXT_LIST_R_USER
		j=0;
		while(buf[cont]!='\n' && buf[cont]!='\0'){lista_texto[TEXT_LIST_R_USER+i][j]=buf[cont]; j++; cont++;}
		lista_texto[TEXT_LIST_R_USER+i][j]='\0';
		cont++; // Nos saltamos el '\n'
	}
	
    TTF_SizeText(fntArial12, lista_texto[TEXT_LIST_R_USER+l_mas_larga], &width, &height);
	width+=10;
    box_h = (height + 4) * (lines+1) + 16; // Una l�nea m�s para el Cerrar, y 6 extra tambi�n para el Cerrar
    
    
	DIALOG *message=malloc(sizeof(DIALOG)*(lines+4));
	if(message==NULL){debug_printf("Error en el malloc del mensaje. Tama�o necesitado: %i\n",lines+4); return;}
	
	message[0].df = d_box_proc;
	message[0].x = scr_width/2-width/2;
	message[0].y = scr_height/2-box_h/2;
	message[0].w = width;
	message[0].h = box_h;
	message[0].fg.r = 0;
	message[0].fg.g = 0;
	message[0].fg.b = 0;
	message[0].fg.a = 255;
	message[0].bg.r = 255;
	message[0].bg.g = 255;
	message[0].bg.b = 255;
	message[0].bg.a = 255;
	message[0].key = 0;
	message[0].flag = 0;
	message[0].d1 = 0;
	message[0].d2 = 0;
	message[0].dp = NULL;
	message[0].dp2 = NULL;
	message[0].dp3 = NULL;
	
	message[1].df = d_button_proc;
	message[1].x = scr_width/2 - width/2 + 6;
	message[1].y = scr_height/2 + box_h/2 - height - 10;
	message[1].w = 0;
	message[1].h = 0;
	message[1].fg.r = 255;
	message[1].fg.g = 255;
	message[1].fg.b = 255;
	message[1].fg.a = 255;
	message[1].bg.r = 64;
	message[1].bg.g = 128;
	message[1].bg.b = 64;
	message[1].bg.a = 255;
	message[1].key = 0;
	message[1].flag = 0;
	message[1].d1 = 0;
	message[1].d2 = 0;
	message[1].dp = "Cerrar";
	message[1].dp2 = fntArial12;
	message[1].dp3 = NULL;
	
	for(i=2; i<lines+2; i++)
	{
		message[i].df = d_label_proc;
		message[i].x = scr_width/2 - width/2 + 5;
		message[i].y = scr_height/2-box_h/2 + (i-2) * (height + 4);
		message[i].w = 0;
		message[i].h = 0;
		message[i].fg.r = 255;
		message[i].fg.g = 255;
		message[i].fg.b = 255;
		message[i].fg.a = 255;
		message[i].bg.r = 0;
		message[i].bg.g = 0;
		message[i].bg.b = 0;
		message[i].bg.a = 0;
		message[i].key = 0;
		message[i].flag = 0;
		message[i].d1 = 0;
		message[i].d2 = 0;
		message[i].dp = lista_texto[TEXT_LIST_R_USER+i-2];
		message[i].dp2 = fntArial12;
		message[i].dp3 = NULL;
	}
	
	message[i].df = NULL;
	
	glColor4f(1.0,1.0,1.0,1.0f);
	do_dialog(message);
	free(message);
}

static void draw_object_list_base(t_obj_base **lista, int contador)
{
	int i;
	for (i=0; i<contador; i++)
	{
		glColor4f(1.0,1.0,1.0,1.0f);
		object_draw_l(lista[i]);
	}
	/* Para la transparencia deben estar los objetos ya dibujados... */
	for (i=0; i<contador; i++)
	{
		if (lista_objeto_base[i]->selec != 0)
		{
			object_draw_selected(lista_objeto_base[i]);
		}
		object_draw_nodes(lista[i]);
	}
}


void resize_window(int width, int height)
{
	//const float ar = (float) width / (float) height;
	/* NORMAL */
	glViewport(0, 0, width, height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,fogRange[1]+100.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void draw_grid(t_heightmap map, float pos_x, float pos_y)
{
	if (pos_x<=-map.tam_x*map.scale/2 || pos_y<=-map.tam_y*map.scale/2 || pos_x>=map.tam_x*map.scale/2 || pos_y>=map.tam_y*map.scale/2)return;
	VECTOR v1, v2, v3;
	float i;
	int sg1, sg2;
	float x_offset, y_offset; /* Como los tri�ngulos no miden un n�mero ex�cto (natural) de metros, dependiendo
	de las coordenadas estar�n desplazados unas d�cimas de metro */
	//const float cos45 = cos(M_PI_4);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	get_current_triangle(marte, pos_x, pos_y, &v1, &v2, &v3);
	/* v1-> 90�; v2->Horizontal(x); v3->Vertical(y) */
	glPointSize(8.0f);
	glBegin(GL_POINTS);
		glColor4f(0.0,0.0,1.0,0.5f);
		glVertex3f(v1.x,v1.y,v1.z+0.1);
		glColor4f(0.0,1.0,0.0,0.5f);
		glVertex3f(v2.x,v2.y,v2.z+0.1);
		glColor4f(1.0,0.0,0.0,0.5f);
		glVertex3f(v3.x,v3.y,v3.z+0.1);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
	glLineWidth(1.0f);
	sg1=nsgn(v2.x-v1.x);
	sg2=nsgn(v3.y-v1.y);
	
	
	x_offset = (map.scale-nfloor(map.scale))*/* Distancia_extra*n�casillas al central -> pasado a [0-1) */
			 (pos_a_casilla_x(map,pos_x)-pos_a_casilla_x(map,-map.ini_x));
	x_offset = x_offset-nfloor(x_offset);
	
	y_offset = (map.scale-nfloor(map.scale))*
			 (pos_a_casilla_y(map,pos_y)-pos_a_casilla_y(map,-map.ini_y));
	y_offset = y_offset-nfloor(y_offset);
	
	
	if (sg1>0){x_offset=(1.33-nabs(x_offset));}else{x_offset+=0.25;}
	if (sg2<0){y_offset=(1.53-nabs(y_offset));}else{y_offset+=0.25;}
	/* Seguro que no funciona bien al final... esto de prueba-error no suele acabar bien */
	
	//#warning Esto no funciona bien...
	
	glColor4f(0.0f,0.0f,1.0f,0.5f);
	for (i=x_offset; i<map.scale+x_offset; i++) // map.scale es el tama�o en metros de una casilla, y por desgracia es float
	{
		glBegin(GL_LINES);
			/* Vertical */
			glVertex3f(	v1.x +i*sg1,
						v1.y,
						v1.z + (i/(float)(v2.x-v1.x))*(v2.z-v1.z)*sg1);
						
			glVertex3f(	v1.x +i*sg1,
						v3.y + (i/(float)(v2.x-v1.x))*(v2.y-v3.y)*sg1,
						v3.z + (i/(float)(v2.x-v1.x))*(v2.z-v3.z)*sg1);
		glEnd();
	}
	for (i=y_offset; i<map.scale; i++)
	{
		glBegin(GL_LINES);
			/* Horizontal */
			glVertex3f(	v1.x,
						v1.y +i*sg2,
						v1.z + (i/(float)(v3.y-v1.y))*(v3.z-v1.z)*sg2);
						
			glVertex3f(	v2.x + (i/(float)(v3.y-v1.y))*(v1.x-v2.x)*sg2,
						v1.y +i*sg2,
						v2.z + (i/(float)(v3.y-v1.y))*(v3.z-v2.z)*sg2);
		glEnd();
	}
}

void display(void)
{
	glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    //glRotatef(-camera.pitch, 1.0f,0,0);
    //glRotatef(-camera.yaw, 0,0,1.0f);
    //glRotatef(-camera.roll, sin(RAD(-camera.yaw)),cos(RAD(-camera.yaw)),-sin(RAD(camera.pitch)));
    
	
	glRotatef(-90, 1,0,0); // Arreglamos el terreno porque estaba siempre torcido
	
    glRotatef(-camera.roll,   0,1,0);
    glRotatef(-camera.pitch, 1,0,0);
	glRotatef(-camera.yaw,  0,0,1);
	/* Dibujos est�ticos (Sky) */
	
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	draw_fixsprite(sun.position[0],sun.position[1],sun.position[2]+SUN_SIZE,sun_texture,SUN_SIZE);
	//glBlendFunc(GL_ONE,GL_ZERO);
	//glDisable(GL_BLEND);
	
	/* Dibujo normal */
	glTranslatef(-camera.pos_x,-camera.pos_y,-camera.pos_z);
    glEnable(GL_LIGHTING);
    glEnable(GL_FOG);
	
	
    glLightfv(GL_LIGHT0, GL_POSITION, sun.position);
    
	glCallList(marte.list);/* Dibujamos el terreno */
	
	/* Dibujamos los objetos */
	draw_object_list_base(lista_objeto_base, lista_objetos_base);
	
	
	/* Dibujamos los extras del terreno */
	
	//draw_sprite (0, 0, 10, sun_texture, 5);
	
	/* - Display Casillas - (casillas solo de la zona actual, casillas de 1x1m^2) */
   	if (camera.show_grid)
	{
		//draw_grid(marte, camera.pos_x, camera.pos_y-marte.scale/2);
		draw_grid(marte, camera.pos_x, camera.pos_y);
	}
	glColor3d(1.0,1.0,1.0);
	
	if (camera.show_grid) /* Ahora muestra la cuadr�cula */
	{
		glCallList(marte.list+1);
	}
	if (camera.show_presion) /* Ahora muestra las normales */
	{
		glCallList(marte.list+2);
	}
	
	glColor3d(1.0,1.0,1.0);
	
	
	
	
    glDisable(GL_LIGHTING);
    
	/* Dibujamos el HUD */
	draw_HUD();
	hud_printf (12, 10*12, "Altura hasta el suelo: %f",altura_al_suelo(marte,camera.pos_x,camera.pos_y,camera.pos_z));
	//hud_printf (12, 11*12, "FPS: %3.2f",FPS);
	
}


void video_init(void)
{
	scr_info = SDL_GetVideoInfo();
	
	if(scr_bpp<=0)scr_bpp = scr_info->vfmt->BitsPerPixel; /* Usamos la config del escritorio */
	
	/* PREVIO del GL*/
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	if(scr_flags<=0)
	{
		scr_flags = SDL_OPENGL | SDL_FULLSCREEN;
	}
	else
	{
		scr_flags |= SDL_OPENGL; // Nos aseguramos que est� definido
	}
	
	/* Laarga lista de resoluciones posibles xD incluso para 16:9 . He usado las m�s comunes, fuente: http://en.wikipedia.org/wiki/Display_resolution */
	if(scr_width <= 0 || scr_height <=0)
	{
		scr_width = 1680;
		scr_height = 1050;
	}
	/* Hmmm he leido hace poco que SDL emula un modo si no lo puede hacer.. as� que a la mierda la lista */
	screen = SDL_SetVideoMode(scr_width, scr_height, scr_bpp, scr_flags);
	if ( screen == NULL )
	{
		scr_width = 1440;
		scr_height = 900;
		screen = SDL_SetVideoMode(scr_width, scr_height, scr_bpp, scr_flags);
		if ( screen == NULL )
		{
			scr_width = 1280;
			scr_height = 1024;
			screen = SDL_SetVideoMode(scr_width, scr_height, scr_bpp, scr_flags);
			if ( screen == NULL )
			{
				scr_width = 1280;
				scr_height = 800;
				screen = SDL_SetVideoMode(scr_width, scr_height, scr_bpp, scr_flags);
				if ( screen == NULL )
				{
					scr_width = 1024;
					scr_height = 768;
					screen = SDL_SetVideoMode(scr_width, scr_height, scr_bpp, scr_flags);
					if ( screen == NULL )
					{
						scr_width = 800;
						scr_height = 600;
						screen = SDL_SetVideoMode(scr_width, scr_height, scr_bpp, scr_flags);
						if ( screen == NULL )
						{
							debug_printf("No se ha podido iniciar la pantalla: %s\n", SDL_GetError());
							exit(1);
						}
					}
				}
			}
		}
	}
	resize_window(scr_width,scr_height);
	
	if (TTF_Init()!=0)
	{
		debug_printf("No se ha podido iniciar el m�dulo de fuentes SDL_TTF: %s.\n", TTF_GetError());
		exit(-1);
	}
	
	/* Funciones por mensaje */
	//atexit(salir);

	/* DevIL init */
	if (ilGetInteger(IL_VERSION_NUM) != IL_VERSION ||
	  iluGetInteger(ILU_VERSION_NUM) != ILU_VERSION ||
	  ilutGetInteger(ILUT_VERSION_NUM) != ILUT_VERSION)
	{
		debug_printf("ERROR, la versi�n de devIL no coincide\n");
		exit(-2);
	} 
	
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	
	/* OpenGL - Specific */
	glShadeModel(GL_SMOOTH);
	glClearColor(0.02f,0.1f,0.02f,1.0f);
	
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, 0.32f);
	glHint(GL_FOG_HINT, GL_DONT_CARE);
	glFogf(GL_FOG_START, fogRange[0]);
	glFogf(GL_FOG_END, fogRange[1]);
	glEnable(GL_FOG);
	
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
    glEnable(GL_CULL_FACE);
    //glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
	
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  sun.ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  sun.diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, sun.specular);
    glLightfv(GL_LIGHT0, GL_POSITION, sun.position);
}
