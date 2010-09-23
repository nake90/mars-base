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

/** \file display.h
 * \brief Funciones y definiciones de control de salida por pantalla
 * Este archivo se encarga de todas las definiciones, structs, funciones, ... que son necesarias para
 * mostrar datos por pantalla. Contiene funciones de iniciación, de mantenimiento, de control, de
 * display, de limpieza de pantalla.
 * \author Alfonso Arbona Gimeno
*/
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <SDL/SDL_ttf.h>
#define ILUT_USE_OPENGL
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
//#include <stdio.h>
//#include <math.h>
//#include <time.h>
#include "atmosferico.h"
#include "objetos.h"
#include "display.h"
#include "overlay.h"
#include "heightmap.h"
#include "control.h"

int scr_width = -1;
int scr_height = -1;
int scr_bpp;
int scr_flags = -1;
/*
PFNGLMULTITEXCOORD1FARBPROC	glMultiTexCoord1fARB	= NULL;
PFNGLMULTITEXCOORD2FARBPROC	glMultiTexCoord2fARB	= NULL;
PFNGLMULTITEXCOORD3FARBPROC	glMultiTexCoord3fARB	= NULL;
PFNGLMULTITEXCOORD4FARBPROC	glMultiTexCoord4fARB	= NULL;
PFNGLACTIVETEXTUREARBPROC	glActiveTextureARB	= NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC	glClientActiveTextureARB= NULL;
*/
void message_printf(const char *fmt, ...)
{
    char buf[1024];
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
            if(max_len_size<cont)
            {
                max_len_size=cont;
                l_mas_larga=lines-1;
            }
            cont=-1;
            lines++;
        }
        cont++;
        i++;
    }
    if(max_len_size<cont)
    {
        max_len_size=cont;    // Si no está esto la última línea no se tiene en cuenta para el tamaño
        l_mas_larga=lines-1;
    }

    int width, height, box_h;

    cont=0;
    int j;
    for(i=0; i<lines; i++)
    {
        // Usamos el buffer 'lista_texto' empezando por TEXT_LIST_R_USER
        j=0;
        while(buf[cont]!='\n' && buf[cont]!='\0')
        {
            lista_texto[TEXT_LIST_R_USER+i][j]=buf[cont];
            j++;
            cont++;
        }
        lista_texto[TEXT_LIST_R_USER+i][j]='\0';
        cont++; // Nos saltamos el '\n'
    }

    TTF_SizeText(fntArial12, lista_texto[TEXT_LIST_R_USER+l_mas_larga], &width, &height);
    width+=10;
    box_h = (height + 4) * (lines+1) + 16; // Una línea más para el Cerrar, y 6 extra también para el Cerrar


    DIALOG *message=malloc(sizeof(DIALOG)*(lines+4));
    if(message==NULL)
    {
        debug_printf(TL_ERR_MALLOC,"message_printf");
        return;
    }

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
    message[1].dp = lista_texto[TEXT_LIST_R_DLG + 0];
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
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.2f,fogRange[1]+100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

static
float interpolate_height(int x1, int y1, int x2, int y2)
{
	float h1, h2;
	if(x1<0){x1=0;}
	if(x2<0){x2=0;}
	if(x1>marte.tam_x-1){x1=marte.tam_x-1;}
	if(x2>marte.tam_x-1){x2=marte.tam_x-1;}
	if(y1<0){y1=0;}
	if(y2<0){y2=0;}
	if(y1>marte.tam_y-1){y1=marte.tam_y-1;}
	if(y2>marte.tam_y-1){y2=marte.tam_y-1;}
	
	if(x1==x2 && y1==y2){return (float)(marte.vertex[x1+y1*marte.tam_x].altura);}
	
	h1=(float)(marte.vertex[x1+y1*marte.tam_x].altura);
	h2=(float)(marte.vertex[x2+y2*marte.tam_x].altura);
	
	return 0.5f * (h2-h1) + h1;
}


static
void quad_draw(unsigned int size_out, unsigned int size_in, unsigned int level)
{
	int cx = (int)(marte.tam_x - (camera.pos_x/marte.scale));
	int cy = (int)(camera.pos_y/marte.scale);
	int inix, iniy, endx, endy;
	int x,y;
	int interpolate=0, intpol=0; // Interpolate para saber si hay que interpolar en los horizontales y intpol para los verticales
	const float v_scale = (marte.max_h-marte.min_h)/255.0f;
	
	#define set_luz(c) if(c<=0) \
		{glColor3f(1,1,1);} \
		else if(c<=1) \
		{glColor3f(c,c,c);} \
		else \
		{glColor3f(1,1,1);}
	
	
	// Dibujamos el terreno
	inix = cx-size_out    +level;
	iniy = cy-size_out    +level;
	endx = cx+size_out+1  -level;
	endy = cy+size_out    -level;
	if(inix < 0)inix=0;
	if(iniy < 0)iniy=0;
	if(endx > marte.tam_x)endx=marte.tam_x;
	if(endy > marte.tam_y-1)endy=marte.tam_y-1;
	
	for(y=iniy; y<endy;y+=level)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for(x=inix; x<endx;x+=level)
		{
			if(level > 1 &&
				 x > cx-size_in && x < cx+size_in-1 &&
				 y > cy-size_in-1 && y < cy+size_in)
			{
				glEnd();
				x=cx+size_in-level;
				glBegin(GL_TRIANGLE_STRIP);
			}
			else
			{
				glTexCoord2f(x*marte.scale, y*marte.scale);
				set_luz(marte.vertex[x+y*marte.tam_x].shadow);
				glVertex3d((marte.tam_x - (x))*marte.scale, (y)*marte.scale, marte.vertex[x+y*marte.tam_x].altura*v_scale);
				
				glTexCoord2f(x*marte.scale, (y+level)*marte.scale);
				set_luz(marte.vertex[x+(y+level)*marte.tam_x].shadow);
				glVertex3d((marte.tam_x - (x))*marte.scale, (y+level)*marte.scale, marte.vertex[x+(y+level)*marte.tam_x].altura*v_scale);
			}
		}
		glEnd();
	}
	
	// Dibujamos las transiciones (Exteriores al quad grande)
	inix -= level;
	iniy -= level;
	endx += level;
	endy += level;
	if(inix < 0)inix=0;
	if(iniy < 0)iniy=0;
	if(endx > marte.tam_x)endx=marte.tam_x;
	if(endy > marte.tam_y-1)endy=marte.tam_y-1;
	
	intpol=0;
	for(y=iniy; y<endy;y+=level) // Para cada 'y' del quad
	{
		if(y==iniy ) // Es borde superior
		{
			interpolate=0;
			glBegin(GL_TRIANGLE_STRIP);
			
			// Esquina (tiene 2 interpolates)
			glTexCoord2f((marte.tam_x - (inix))*marte.scale, (y)*marte.scale);
			set_luz(marte.vertex[(inix)+(y)*marte.tam_x].shadow);
			glVertex3d((marte.tam_x - (inix))*marte.scale, (y)*marte.scale, marte.vertex[(inix)+(y)*marte.tam_x].altura*v_scale);
			
			glTexCoord2f((marte.tam_x - (inix))*marte.scale, (y+level)*marte.scale);
			set_luz(marte.vertex[(inix)+(y+level)*marte.tam_x].shadow);
			glVertex3d((marte.tam_x - (inix))*marte.scale, (y+level)*marte.scale, interpolate_height(inix,y,inix,y+level+level)*v_scale);
			
			for(x=inix+level; x<endx-level;x+=level)
			{
				if(interpolate>0){interpolate=0;}else{interpolate=level;}
				glTexCoord2f((marte.tam_x - (x))*marte.scale, (y)*marte.scale);
				set_luz(marte.vertex[(x)+(y)*marte.tam_x].shadow);
				glVertex3d((marte.tam_x - (x))*marte.scale, (y)*marte.scale, interpolate_height(x-interpolate, y,x+interpolate,y)*v_scale);
				
				glTexCoord2f((marte.tam_x - (x))*marte.scale, (y+level)*marte.scale);
				set_luz(marte.vertex[(x)+(y+level)*marte.tam_x].shadow);
				glVertex3d((marte.tam_x - (x))*marte.scale, (y+level)*marte.scale,marte.vertex[x+(y+level)*marte.tam_x].altura*v_scale);
			}
			
			// Esquina (tiene 2 interpolates)
			glTexCoord2f((marte.tam_x - (endx-1))*marte.scale, (y)*marte.scale);
			set_luz(marte.vertex[(endx-1)+(y)*marte.tam_x].shadow);
			glVertex3d((marte.tam_x - (endx-1))*marte.scale, (y)*marte.scale, marte.vertex[(endx-1)+(y)*marte.tam_x].altura*v_scale);
			
			glTexCoord2f((marte.tam_x - (endx-1))*marte.scale, (y+level)*marte.scale);
			set_luz(marte.vertex[(endx-1)+(y+level)*marte.tam_x].shadow);
			glVertex3d((marte.tam_x - (endx-1))*marte.scale, (y+level)*marte.scale, interpolate_height(endx-1,y,endx-1,y+level+level)*v_scale);
			
			glEnd();
		}
		else if(y==endy-level) // Es borde inferior
		{
			interpolate=0;
			glBegin(GL_TRIANGLE_STRIP);
			
			// Esquina (tiene 2 interpolates)
			glTexCoord2f((marte.tam_x - (inix))*marte.scale, (y)*marte.scale);
			set_luz(marte.vertex[(inix)+(y)*marte.tam_x].shadow);
			glVertex3d((marte.tam_x - (inix))*marte.scale, (y)*marte.scale, interpolate_height(inix,y-level,inix,y+level)*v_scale);
			
			glTexCoord2f((marte.tam_x - (inix))*marte.scale, (y+level)*marte.scale);
			set_luz(marte.vertex[(inix)+(y+level)*marte.tam_x].shadow);
			glVertex3d((marte.tam_x - (inix))*marte.scale, (y+level)*marte.scale, marte.vertex[(inix)+(y+level)*marte.tam_x].altura*v_scale);
			
			for(x=inix+level; x<endx-level;x+=level)
			{
				if(interpolate>0){interpolate=0;}else{interpolate=level;}
				glTexCoord2f((marte.tam_x - (x))*marte.scale, (y)*marte.scale);
				set_luz(marte.vertex[(x)+(y)*marte.tam_x].shadow);
				glVertex3d((marte.tam_x - (x))*marte.scale, (y)*marte.scale, marte.vertex[x+y*marte.tam_x].altura*v_scale);
				
				glTexCoord2f((marte.tam_x - (x))*marte.scale, (y+level)*marte.scale);
				set_luz(marte.vertex[(x)+(y+level)*marte.tam_x].shadow);
				glVertex3d((marte.tam_x - (x))*marte.scale, (y+level)*marte.scale, interpolate_height(x-interpolate, y+level,x+interpolate,y+level)*v_scale);
			}
			
			// Esquina (tiene 2 interpolates)
			glTexCoord2f((marte.tam_x - (endx-1))*marte.scale, (y)*marte.scale);
			set_luz(marte.vertex[(endx-1)+(y)*marte.tam_x].shadow);
			glVertex3d((marte.tam_x - (endx-1))*marte.scale, (y)*marte.scale, interpolate_height(endx-1,y-level,endx-1,y+level)*v_scale);
			
			glTexCoord2f((marte.tam_x - (endx-1))*marte.scale, (y+level)*marte.scale);
			set_luz(marte.vertex[(endx-1)+(y+level)*marte.tam_x].shadow);
			glVertex3d((marte.tam_x - (endx-1))*marte.scale, (y+level)*marte.scale, marte.vertex[(endx-1)+(y+level)*marte.tam_x].altura*v_scale);
			
			glEnd();
		}
		else// Es borde lateral
		{
			glBegin(GL_TRIANGLE_STRIP); // Borde izquierdo
				if(intpol==0)
				{
					glTexCoord2f((marte.tam_x - (inix))*marte.scale, (y)*marte.scale);
					set_luz(marte.vertex[(inix)+(y)*marte.tam_x].shadow);
					glVertex3d((marte.tam_x - (inix))*marte.scale, (y)*marte.scale, interpolate_height(inix,y-level,inix,y+level)*v_scale);
					
					glTexCoord2f((marte.tam_x - (inix))*marte.scale, (y+level)*marte.scale);
					set_luz(marte.vertex[(inix)+(y+level)*marte.tam_x].shadow);
					glVertex3d((marte.tam_x - (inix))*marte.scale, (y+level)*marte.scale, marte.vertex[(inix)+(y+level)*marte.tam_x].altura*v_scale);
				}
				else
				{
					glTexCoord2f((marte.tam_x - (inix))*marte.scale, (y)*marte.scale);
					set_luz(marte.vertex[(inix)+(y)*marte.tam_x].shadow);
					glVertex3d((marte.tam_x - (inix))*marte.scale, (y)*marte.scale, marte.vertex[(inix)+y*marte.tam_x].altura*v_scale);
					
					glTexCoord2f((marte.tam_x - (inix))*marte.scale, (y+level)*marte.scale);
					set_luz(marte.vertex[(inix)+(y+level)*marte.tam_x].shadow);
					glVertex3d((marte.tam_x - (inix))*marte.scale, (y+level)*marte.scale, interpolate_height(inix,y,inix,y+level+level)*v_scale);
				}
				glTexCoord2f((marte.tam_x - (inix+level))*marte.scale, (y)*marte.scale);
				set_luz(marte.vertex[(inix+level)+(y)*marte.tam_x].shadow);
				glVertex3d((marte.tam_x - (inix+level))*marte.scale, (y)*marte.scale, marte.vertex[(inix+level)+y*marte.tam_x].altura*v_scale);
				
				glTexCoord2f((marte.tam_x - (inix+level))*marte.scale, (y+level)*marte.scale);
				set_luz(marte.vertex[(inix+level)+(y+level)*marte.tam_x].shadow);
				glVertex3d((marte.tam_x - (inix+level))*marte.scale, (y+level)*marte.scale, marte.vertex[(inix+level)+(y+level)*marte.tam_x].altura*v_scale);
			glEnd();
			glBegin(GL_TRIANGLE_STRIP); // Borde derecho
				glTexCoord2f((marte.tam_x - (endx-1-level))*marte.scale, (y)*marte.scale);
				set_luz(marte.vertex[(endx-1-level)+(y)*marte.tam_x].shadow);
				glVertex3d((marte.tam_x - (endx-1-level))*marte.scale, (y)*marte.scale, marte.vertex[(endx-1-level)+y*marte.tam_x].altura*v_scale);
				
				glTexCoord2f((marte.tam_x - (endx-1-level))*marte.scale, (y+level)*marte.scale);
				set_luz(marte.vertex[(endx-1-level)+(y+level)*marte.tam_x].shadow);
				glVertex3d((marte.tam_x - (endx-1-level))*marte.scale, (y+level)*marte.scale, marte.vertex[(endx-1-level)+(y+level)*marte.tam_x].altura*v_scale);
				if(intpol==0)
				{
					glTexCoord2f((marte.tam_x - (endx-1))*marte.scale, (y)*marte.scale);
					set_luz(marte.vertex[(endx-1)+(y)*marte.tam_x].shadow);
					glVertex3d((marte.tam_x - (endx-1))*marte.scale, (y)*marte.scale, interpolate_height((endx-1),y-level,(endx-1),y+level)*v_scale);
					
					glTexCoord2f((marte.tam_x - (endx-1))*marte.scale, (y+level)*marte.scale);
					set_luz(marte.vertex[(endx-1)+(y+level)*marte.tam_x].shadow);
					glVertex3d((marte.tam_x - (endx-1))*marte.scale, (y+level)*marte.scale, marte.vertex[(endx-1)+(y+level)*marte.tam_x].altura*v_scale);
				}
				else
				{
					glTexCoord2f((marte.tam_x - (endx-1))*marte.scale, (y)*marte.scale);
					set_luz(marte.vertex[(endx-1)+(y)*marte.tam_x].shadow);
					glVertex3d((marte.tam_x - (endx-1))*marte.scale, (y)*marte.scale, marte.vertex[(endx-1)+y*marte.tam_x].altura*v_scale);
					
					glTexCoord2f((marte.tam_x - (endx-1))*marte.scale, (y+level)*marte.scale);
					set_luz(marte.vertex[(endx-1)+(y+level)*marte.tam_x].shadow);
					glVertex3d((marte.tam_x - (endx-1))*marte.scale, (y+level)*marte.scale, interpolate_height((endx-1),y,(endx-1),y+level+level)*v_scale);
				}
			glEnd();
			if(intpol == 0){intpol = 1;}else{intpol = 0;}
		}
	}
	
	#undef set_luz
}

void draw_heightmap(void)
{
	// OpenGL setup
	
	glColor3f(1.0f,1.0f,1.0f);
	use_texture(sand);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	
	if(camera.show_grid)glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	quad_draw(4, 0, 1);
	quad_draw(16, 4, 2);
	quad_draw(64, 16, 4);
	quad_draw(256, 64, 8);
	if(camera.show_grid)glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	//glCallList(marte.list); // A la antigüa
}

void display(void)
{
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);

    glRotatef(-camera.roll,   0.0f,1.0f,0.0f);
    glRotatef(-camera.pitch, 1.0f,0.0f,0.0f);
    glRotatef(-camera.yaw,  0.0f,0.0f,1.0f);
    /* Dibujos estáticos (Sky) */

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    draw_fixsprite(sun.position[0],sun.position[1],sun.position[2]+SUN_SIZE,sun.material,SUN_SIZE);
    //glBlendFunc(GL_ONE,GL_ZERO);
    //glDisable(GL_BLEND);
    
    /* Dibujo normal */
    glTranslatef(-camera.pos_x,-camera.pos_y,-camera.pos_z);
    glEnable(GL_LIGHTING);
    glEnable(GL_FOG);
    glLightfv(GL_LIGHT0, GL_POSITION, sun.position);
    
    
    /*glPushMatrix();
    glTranslatef(163870.0,139795.0,3484.0);
    glColor3f(1.0,1.0,1.0);
    use_texture(sand);
    glBegin(GL_QUADS);
			glTexCoord2d(-5.0, 10.0);
			glVertex3d(-5.0, 10.0, 10.0);
			
			glTexCoord2d(-5.0, 0.0);
			glVertex3d(-5.0, 10.0, 0.0);
			
			glTexCoord2d(5.0, 0.0);
			glVertex3d(+5.0, 10.0, 0.0);
			
			glTexCoord2d(5.0, 10.0);
			glVertex3d(+5.0, 10.0, 10.0);
		glEnd();
		glPopMatrix();*/
    
    draw_heightmap();

    /* Dibujamos los objetos */
    draw_object_list_base(lista_objeto_base, lista_objetos_base);

    /* Dibujamos los extras del terreno */
    //draw_sprite (0, 0, 10, sun.material, 5);

    glColor3d(1.0,1.0,1.0);
    VECTOR coord, pos={camera.pos_x, camera.pos_y, camera.pos_z};
    COLORf dcolor = {0.0f, 0.0f, 1.0f, 1.0f};
    if(get_traced_coord(pos,get_mouse_vector(),&coord)!=0)
    {
    	str_cpy(TL_DEBUG1, "Error al calcular las coordenadas de corte");
    }
    else
    {
    	sprintf(TL_DEBUG1, "Coordenadas de corte: (%.2f, %.2f, %.2f)", coord.x, coord.y, coord.z);
    }
    debug_point_3D(coord,dcolor,10.0f);


    /* Dibujamos el HUD */
    glColor3d(1.0,1.0,1.0);
    glDisable(GL_LIGHTING);
    draw_HUD();
}


void video_init(void)
{
  scr_info = SDL_GetVideoInfo();

  if(scr_bpp<=0)scr_bpp = scr_info->vfmt->BitsPerPixel; /* Usamos la config del escritorio */

  /* PREVIO del GL*/
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
  if(scr_flags<=0)
  {
		ifdebug(DEBUG_DEBUG)
		{
			debug_printf("Fullscreen\n");
		}
		scr_flags = SDL_OPENGL | SDL_FULLSCREEN;
  }
  else
  {
		ifdebug(DEBUG_DEBUG)
		{
			debug_printf("Windowed\n");
		}
		scr_flags |= SDL_OPENGL; // Nos aseguramos que está definido
  }

  /* Larga lista de resoluciones posibles xD incluso para 16:9 . He usado las más comunes, fuente: http://en.wikipedia.org/wiki/Display_resolution */
  if(scr_width <= 0 || scr_height <=0)
  {
		scr_width = 1680;
		scr_height = 1050;
  }
  /* Hmmm he leido hace poco que SDL emula un modo si no lo puede hacer.. así que a la mierda la lista */
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
							debug_printf(TL_ERR_VIDEO_INIT, SDL_GetError());
							exit(1);
						}
					}
				}
			}
		}
  }
  resize_window(scr_width,scr_height);
  ifdebug(DEBUG_DEBUG)
  {
		debug_printf("Screenres %ix%ix%i\n",scr_width,scr_height,scr_bpp);
  }

  ifdebug(DEBUG_DEBUG)
  {
		debug_printf("TTF_Init\n");
  }
  if (TTF_Init()!=0)
  {
		debug_printf(TL_ERR_TTF_INIT, TTF_GetError());
		exit(-1);
  }

  /* Funciones por mensaje */
  //atexit(salir);


  ifdebug(DEBUG_DEBUG)
  {
		debug_printf("GL init\n");
  }
  /* OpenGL - Specific */
  //if(!isExtensionSupported("GL_ARB_multitexture")){debug_printf("ERROR, GL_ARB_multitexture not supported"); exit(-3);}

  ifdebug(DEBUG_INFO) debug_printf(" OpenGL version: %s\n\n", glGetString(GL_VERSION) );

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

  //glDisable(GL_CULL_FACE);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glEnable(GL_LIGHT0);
  //glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);

  glLightfv(GL_LIGHT0, GL_AMBIENT,  sun.ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  sun.diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, sun.specular);
  glLightfv(GL_LIGHT0, GL_POSITION, sun.position);

  //glEnable (GL_TEXTURE_2D);
  //glMatrixMode (GL_PROJECTION);
  //glLoadIdentity();

  ifdebug(DEBUG_DEBUG)
  {
      debug_printf("video_init OK\n");
  }
}

