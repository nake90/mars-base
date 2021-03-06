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

/** \file overlay.c
 * \brief Controla el HUD
 * \author Alfonso Arbona Gimeno
*/

#include "control.h"
#include "overlay.h"
#include "heightmap.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#define ILUT_USE_OPENGL
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <math.h>
#include "atmosferico.h"
#include "objetos.h"

void draw_minimap(GLuint minimap)
{
    int w,h;
    if (!minimap)return;
    set_gl_mode();

    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, minimap);

    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);

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
    x = (marte.tam_x-(camera.pos_x/marte.scale))/marte.tam_x  *w/4 +scr_width-w/4-2;
    y = (camera.pos_y/marte.scale)/marte.tam_y  *h/4 +2;

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
    glVertex2f(x+sin(RAD(-camera.yaw-22.5+180))*20 +cos(RAD(-camera.yaw+180))*20,
               +y-cos(RAD(camera.yaw+22.5+180))*20 +sin(RAD(-camera.yaw+180))*20);
    glVertex2f(x+sin(RAD(-camera.yaw+22.5+180))*20 -cos(RAD(-camera.yaw+180))*20,
               +y-cos(RAD(camera.yaw-22.5+180))*20 -sin(RAD(-camera.yaw+180))*20);
    glEnd();

    restore_gl_mode();
}

void draw_HUD(void)
{
    /* DIALOG: {(*df), x, y, w, h, fg, bg, key, flag, d1, d2, *dp, *dp2, *dp3} */
    DIALOG box1 = {d_box_proc, 0, 0, 430, 160 ,{0,255,0,128} ,{0,128,0,128}, 0, 0, 0, 0, NULL, NULL, NULL};
    /*DIALOG lbl_test[] = {
    {d_label_proc, 20, 220, 80, 12 ,{255,0,0,255} ,{0,128,0,128}, 0, 0, 0, 0, "Hola mundo1!", fntArial12, NULL},
    {d_label_proc, 20, 232, 80, 12 ,{255,0,0,255} ,{0,128,0,128}, 0, 0, 0, 0, "Hola mundo2!", fntArial12, NULL},
    {d_label_proc, 20, 244, 80, 12 ,{255,0,0,255} ,{0,128,0,128}, 0, 0, 0, 0, "Hola mundo3!", fntArial12, NULL},
    {d_label_proc, 20, 256, 80, 12 ,{255,0,0,255} ,{0,128,0,128}, 0, 0, 0, 0, "Hola mundo4!", fntArial12, NULL},
    {NULL_DIALOG}};

    draw_dialog(lbl_test);*/
    draw_element(box1);

    draw_minimap(minimapa);

    hud_printf (12, 2*12, "Mars_Base(%s) v.%s.%i",INFO_STATUS,INFO_FULLVERSION_STRING,INFO_BUILDS_COUNT);
    hud_printf (12, 3*12, "PITCH, YAW, ROLL = (%.2f, %.2f, %.2f)",camera.pitch, camera.yaw, camera.roll);
    hud_printf (12, 4*12, "POS = (%.2f, %.2f, %.2f)",camera.pos_x, camera.pos_y, camera.pos_z);

    int x,y;
    SDL_GetMouseState(&x,&y);
    float angx = (((scr_width/2.0f) - x)*45.0f) / (scr_width/2.0f)*0.65;
    float angy = (((scr_height/2.0f) - y)*45.0f) / (scr_height/2.0f)*0.5;
    VECTOR pos = {camera.pos_x,camera.pos_y,camera.pos_z};
    VECTOR dir = v_from_ang(RAD(camera.pitch-90.0f), RAD(camera.yaw));
    VECTOR up_axis = v_from_ang(RAD(camera.pitch), RAD(camera.yaw));
    VECTOR side_axis = v_from_ang(0.0f, RAD(camera.yaw-90.0f));
    dir = vrotate_axis(dir, side_axis, RAD(angy));
    dir = vrotate_axis(dir, up_axis, RAD(angx));
    int obj = get_traced_object(pos, dir);

    hud_printf (12, 5*12, "Vector look: (%.2f, %.2f, %.2f), trace: %i",dir.x, dir.y, dir.z, obj);
    //hud_printf (12, 6*12, "C/V -> Ver/ocultar la cuadrícula");
    //hud_printf (12, 7*12, "B/N -> Ver/ocultar las normales");
    
    hud_printf (12, 7*12, lista_texto[TEXT_LIST_R_HUD + 0],altura_al_suelo(marte,camera.pos_x,camera.pos_y,camera.pos_z));
    hud_printf (12, 11*12, TL_FPS);
    
    hud_printf (12, 12*12, TL_DEBUG1);
    hud_printf (12, 13*12, TL_DEBUG2);
    

    int i, cont=0;
    float tot;
    for(i=0; i<lista_objetos_base; i++)
    {
        if(lista_objeto_base[i]->exists && lista_objeto_base[i]->selec == 1)
        {
            tot=moles_gas_total(lista_objeto_base[i]->node_data.gases);
            hud_printf (12, (cont*10+20)*12, "%s (id = %i):", lista_objeto_base[i]->name,i);
            hud_printf (12, (cont*10+21)*12, "  Press: %.3f", get_presion(lista_objeto_base[i]->node_data.gases, lista_objeto_base[i]->node_data.volumen, lista_objeto_base[i]->node_data.temperatura));
            hud_printf (12, (cont*10+22)*12, "  CO2 %3.2f%%", lista_objeto_base[i]->node_data.gases.CO2/tot * 100);
            hud_printf (12, (cont*10+23)*12, "  N2  %3.2f%%", lista_objeto_base[i]->node_data.gases.N2/tot * 100);
            hud_printf (12, (cont*10+24)*12, "  Ar  %3.2f%%", lista_objeto_base[i]->node_data.gases.Ar/tot * 100);
            hud_printf (12, (cont*10+25)*12, "  O2  %3.2f%%", lista_objeto_base[i]->node_data.gases.O2/tot * 100);
            hud_printf (12, (cont*10+26)*12, "  CO  %3.2f%%", lista_objeto_base[i]->node_data.gases.CO/tot * 100);
            hud_printf (12, (cont*10+27)*12, "  H2O %3.2f%%", lista_objeto_base[i]->node_data.gases.H2O/tot * 100);
            cont++;
        }
    }
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
        if(ret!=D_O_K)
        {
            res=ret;
        }
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
        if (d->d1>0)
        {
            glLineWidth(d->d1);
        }
        else
        {
            glLineWidth(1);
        }
        glColor4f(d->fg.r/255.0f, d->fg.g/255.0f, d->fg.b/255.0f, d->fg.a/255.0f);
        glBegin(GL_LINE);
        glVertex2i(d->x, d->y);
        glVertex2i(d->w, d->h);
        glEnd();
        d->flag |= D_DIRTY;
        break;
    case MSG_IDLE:

        break;
    }
    return D_O_K;
}

int d_label_proc(int msg, struct st_Dialog * d, int c)
{
    if(d->dp2==NULL)
    {
        debug_printf("Error, d_label sin font! Dialog cerrado\n");
        return D_CLOSE;
    }
    switch (msg)
    {
    case MSG_START:
        if(d->h==0)
        {
            d->h=TTF_FontHeight((TTF_Font*)d->dp2)+4;
        }
        if(d->w==0)
        {
            TTF_SizeText((TTF_Font*)d->dp2, d->dp, &d->w, NULL);
            d->w += 4;
        }
        break;

    case MSG_END:

        break;
    case MSG_DRAW:
        glColor4f(d->bg.r/255.0f, d->bg.g/255.0f, d->bg.b/255.0f, d->bg.a/255.0f);
        SDL_Color clrFg = {d->fg.r, d->fg.g, d->fg.b, d->fg.a}; /* Color del texto */

        glBegin(GL_QUADS);
        glVertex2i(d->x, d->y);				/* Arriba izquierda */
        glVertex2i(d->x, d->y+d->h);		/* Abajo izquierda */
        glVertex2i(d->x+d->w, d->y+d->h);	/* Abajo derecha */
        glVertex2i(d->x+d->w, d->y);		/* Arriba derecha */
        glEnd();

        SDL_GL_RenderText(d->dp, (TTF_Font*)d->dp2, clrFg, d->x+2, d->y+d->h-2, 0.1);

        d->flag |= D_DIRTY;
        break;
    case MSG_IDLE:

        break;
    }
    return D_O_K;
}

int d_image_proc(int msg, struct st_Dialog * d, int c)
{
    switch (msg)
    {
    case MSG_START:

        break;
    case MSG_END:

        break;
    case MSG_DRAW:
        //glColor4f(d->bg.r/255.0f, d->bg.g/255.0f, d->bg.b/255.0f, d->bg.a/255.0f);
        glColor3f(1.0f, 1.0f, 1.0f);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, d->d1);
        glBegin(GL_QUADS);
        glTexCoord2f(0,1);
        glVertex2i(d->x, d->y);				/* Arriba izquierda */
        glTexCoord2f(0,0);
        glVertex2i(d->x, d->y+d->h);		/* Abajo izquierda */
        glTexCoord2f(1,0);
        glVertex2i(d->x+d->w, d->y+d->h);	/* Abajo derecha */
        glTexCoord2f(1,1);
        glVertex2i(d->x+d->w, d->y);		/* Arriba derecha */
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glLineWidth(2);
        glColor4f(d->fg.r/255.0f, d->fg.g/255.0f, d->fg.b/255.0f, d->fg.a/255.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2i(d->x+1, d->y+1);				/* Arriba izquierda */
        glVertex2i(d->x+1, d->y+d->h-1);		/* Abajo izquierda */
        glVertex2i(d->x+d->w-1, d->y+d->h-1);	/* Abajo derecha */
        glVertex2i(d->x+d->w-1, d->y+1);		/* Arriba derecha */
        glEnd();
        d->flag |= D_DIRTY;
        break;
    case MSG_IDLE:

        break;
    }
    return D_O_K;
}


int d_icon_proc(int msg, struct st_Dialog * d, int c)
{
    static float color_divisor;
    static int border_color;
    if(!color_divisor)
    {
        color_divisor = 255.0f;    // Por si intentamos dibujar sin iniciar
        border_color = 0;
    }
    switch (msg)
    {
    case MSG_START:
        color_divisor = 255.0f;
        border_color = 0;
        break;
    case MSG_END:

        break;
    case MSG_DRAW:
        glColor4f(d->bg.r/color_divisor, d->bg.g/color_divisor, d->bg.b/color_divisor, d->bg.a/255.0f);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, d->d1);
        glBegin(GL_QUADS);
        glTexCoord2f(0,1);
        glVertex2i(d->x, d->y);				/* Arriba izquierda */
        glTexCoord2f(0,0);
        glVertex2i(d->x, d->y+d->h);		/* Abajo izquierda */
        glTexCoord2f(1,0);
        glVertex2i(d->x+d->w, d->y+d->h);	/* Abajo derecha */
        glTexCoord2f(1,1);
        glVertex2i(d->x+d->w, d->y);		/* Arriba derecha */
        glEnd();

        glDisable(GL_TEXTURE_2D);
        glLineWidth(2);
        if(d->flag & D_GOTMOUSE)//border_color == 1)
        {
            glColor4f(1.0f, 1.0f, 1.0f, d->fg.a/255.0f);
        }
        else
        {
            glColor4f(d->fg.r/255.0f, d->fg.g/255.0f, d->fg.b/255.0f, d->fg.a/255.0f);
        }
        glBegin(GL_LINE_LOOP);
        glVertex2i(d->x+1, d->y+1);				/* Arriba izquierda */
        glVertex2i(d->x+1, d->y+d->h-1);		/* Abajo izquierda */
        glVertex2i(d->x+d->w-1, d->y+d->h-1);	/* Abajo derecha */
        glVertex2i(d->x+d->w-1, d->y+1);		/* Arriba derecha */
        glEnd();
        d->flag |= D_DIRTY;
        break;
    case MSG_GOTMOUSE:
        //d->flag |= D_GOTMOUSE;
        //d->flag |= D_DIRTY;
        return D_O_K;

    case MSG_LOSTMOUSE:
        //d->flag &= !D_GOTMOUSE;
        //d->flag |= D_DIRTY;
        return D_O_K;

    case MSG_LPRESS:
        return D_CLOSE;

    case MSG_IDLE:
        //return D_REDRAW; No quieres descomentarlo... seguro
        break;
    }
    return D_O_K;
}

int d_button_proc(int msg, struct st_Dialog * d, int c)
{
    if(d->dp2==NULL)
    {
        debug_printf("Error, d_button sin font! Dialog cerrado\n");
        return D_CLOSE;
    }

    switch (msg)
    {
    case MSG_START:
        if(d->h==0)
        {
            d->h=TTF_FontHeight((TTF_Font*)d->dp2)+4;
        }
        if(d->w==0)
        {
            TTF_SizeText((TTF_Font*)d->dp2, d->dp, &d->w, NULL);
            d->w += 4;
        }
        break;
    case MSG_END:

        break;
    case MSG_DRAW:
        glColor4f(d->bg.r/255.0f, d->bg.g/255.0f, d->bg.b/255.0f, d->bg.a/255.0f);
        SDL_Color clrFg = {d->fg.r, d->fg.g, d->fg.b, d->fg.a}; /* Color del texto */

        glBegin(GL_QUADS);
        glVertex2i(d->x, d->y);				/* Arriba izquierda */
        glVertex2i(d->x, d->y+d->h);		/* Abajo izquierda */
        glVertex2i(d->x+d->w, d->y+d->h);	/* Abajo derecha */
        glVertex2i(d->x+d->w, d->y);		/* Arriba derecha */
        glEnd();

        glColor3f(0.2f, 0.4f, 0.2f); // Claro
        glBegin(GL_LINE_STRIP);
        glVertex2i(d->x+d->w, d->y);		/* Arriba derecha */
        glVertex2i(d->x, d->y);				/* Arriba izquierda */
        glVertex2i(d->x, d->y+d->h);		/* Abajo izquierda */
        glEnd();

        glColor3f(0.15f, 0.2f, 0.15f); // Oscuro
        glBegin(GL_LINE_STRIP);
        glVertex2i(d->x, d->y+d->h);		/* Abajo izquierda */
        glVertex2i(d->x+d->w, d->y+d->h);	/* Abajo derecha */
        glVertex2i(d->x+d->w, d->y);		/* Arriba derecha */
        glEnd();

        SDL_GL_RenderText(d->dp, (TTF_Font*)d->dp2, clrFg, d->x+2, d->y+d->h-2, 0.1);

        d->flag |= D_DIRTY;
        break;

    case MSG_LPRESS:
        return D_CLOSE;

    case MSG_IDLE:
        break;
    }
    return D_O_K;
}

int do_dialog(DIALOG *d)
{
    /* Inicialización */
    int current_id = 0; // Resultado a retornar (id del elemento que ha salido)
    int ret, message, ret_id=-1;
    int status = 1; // 0 -> Cerrar, 1 -> Necesita inicializar, 2 -> Idle, 3 -> Dibujar todo, 4 -> Orden extra
    int message_saved = 0; // Si lo dejo inicializado el compilador hace un warning
    int mstat,x,y; // Coordenadas del ratón
    int id_extra;
    int is_dirty=0;
    SDL_ShowCursor(0); // No mostramos el ratón al guardar la imagen de la pantalla
    set_gl_mode();
    glEnable(GL_TEXTURE_2D);
    /* Guardamos una foto de la pantalla tal como estaba para ponerla de fondo (Que cutre eeh? Pues funciona y es rápido!) */

    ILuint pantalla; // Foto de la pantalla
    ilGenImages(1,&pantalla); // Iniciamos la imagen
    ilBindImage(pantalla); // La seleccionamos
    ilutGLScreen(); // Hacemos la foto y la guardamos en 'pantalla'


    SDL_ShowCursor(1);

    /* Main loop */
    while(status!=0)
    {
        message=0;
        current_id=0;

        if(status == 1)
        {
            message=MSG_START;
            status = 3;
            glEnable(GL_TEXTURE_2D);
            glColor3d(1.0,1.0,1.0);
            ilBindImage(pantalla);
            ilutGLBindTexImage();
            glBegin(GL_QUADS);
            glTexCoord2i(0, 1);
            glVertex2i(0,   0);
            glTexCoord2i(0, 0);
            glVertex2i(0,   scr_height);
            glTexCoord2i(1, 0);
            glVertex2i(scr_width, scr_height);
            glTexCoord2i(1, 1);
            glVertex2i(scr_width, 0);
            glEnd();
            glFinish();
            SDL_GL_SwapBuffers();
        }
        else if(status == 3)
        {
            message=MSG_DRAW;
            status = 2;
        }

        if(message==0) // Si aún no hay un mensaje miramos si hay algún evento
        {
            while(SDL_PollEvent(&sdl_event))
            {
                switch(sdl_event.type)
                {
                case SDL_QUIT:
                    /* Handle quit requests (like Ctrl-c). */
                    exit(0);
                    break;
                case SDL_ACTIVEEVENT: /* No funciona bien... */
                    if(sdl_event.active.state==SDL_APPACTIVE)
                    {
                        window_mode=(sdl_event.active.gain==0?1:0);
                        if(window_mode==0)
                        {
                            SDL_WM_GrabInput(SDL_GRAB_ON);
                        }
                        else
                        {
                            SDL_WM_GrabInput(SDL_GRAB_OFF);
                        }
                        debug_printf("Minimizado: %i\n",window_mode);
                    }
                    break;
                    /*
                    				case SDL_KEYDOWN:
                    key_handle(sdl_event.key.keysym.sym, sdl_event.key.keysym.mod);
                    break;
                    				case SDL_KEYUP:
                    key_up_handle(sdl_event.key.keysym.sym, sdl_event.key.keysym.mod);
                    break;
                    */
                case SDL_MOUSEMOTION: // Se mueve el ratón o se aprieta un botón
                case SDL_MOUSEBUTTONDOWN:
                    mstat = SDL_GetMouseState(&x,&y);
                    id_extra=0;
                    while(d[id_extra].df!=NULL)
                    {
                        // Si está sobre un objeto
                        if((x >= d[id_extra].x && x <= d[id_extra].x + d[id_extra].w) && (y >= d[id_extra].y && y <= d[id_extra].y + d[id_extra].h))
                        {
                            if(mstat & SDL_BUTTON(SDL_BUTTON_LEFT))
                            {
                                if(d[id_extra].df(MSG_LPRESS,&d[id_extra],0)==D_CLOSE)
                                {
                                    status=0;
                                    ret_id=id_extra;
                                }
                            }

                            if(!(d[id_extra].flag & D_GOTMOUSE))
                            {
                                d[id_extra].flag |= D_GOTMOUSE;
                                ret=d[id_extra].df(MSG_GOTMOUSE,&d[id_extra],0);
                                //debug_printf(" MSG -> MSG_GOTMOUSE a %i, ret %i\n",id_extra,ret);
                                message=MSG_DRAW; // Pequeño apañijo para arreglar el problema de got_mouse / lost_mouse

                                switch (ret)
                                {
                                case D_CLOSE:
                                    status=0;
                                    ret_id=id_extra;
                                    break;
                                case D_REDRAW_ALL:
                                case D_REDRAW: // Dibujar todo
                                    message=MSG_DRAW;
                                    break;
                                case D_REDRAWME:
                                    if(d[id_extra].df(MSG_DRAW,&d[id_extra],0)==D_CLOSE)status=0;
                                    break;
                                }
                            }
                        }
                        else // no está en ese objeto
                        {
                            //debug_printf(" MSG -> No está sobre %i, got_mouse = %i\n",id_extra, d[id_extra].flag & D_GOTMOUSE);
                            if(d[id_extra].flag & D_GOTMOUSE)
                            {
                                d[id_extra].flag &= !D_GOTMOUSE;

                                ret=d[id_extra].df(MSG_LOSTMOUSE,&d[id_extra],0);
                                //debug_printf(" MSG -> MSG_LOSTMOUSE a %i, ret %i\n",id_extra,ret);
                                message=MSG_DRAW; // Pequeño apañijo para arreglar el problema de got_mouse / lost_mouse


                                switch (ret)
                                {
                                case D_CLOSE:
                                    status=0;
                                    ret_id=id_extra;
                                    break;
                                case D_REDRAW_ALL:
                                case D_REDRAW: // Dibujar todo
                                    message=MSG_DRAW;
                                    break;
                                case D_REDRAWME:
                                    if(d[id_extra].df(MSG_DRAW,&d[id_extra],0)==D_CLOSE)status=0;
                                    break;
                                }
                            }
                        }
                        id_extra++;
                    }
                }
            }
        }

        if(message==0)
        {
            SDL_Delay(20);    // Aún no hay mensaje? pues IDLE
            message=MSG_IDLE;
        }
        current_id=0;

        if(message==MSG_DRAW)
        {
            glEnable(GL_TEXTURE_2D);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            ilBindImage(pantalla);
            ilutGLBindTexImage();
            glColor3d(1.0,1.0,1.0);
            glBegin(GL_QUADS);
            glTexCoord2i(0, 1);
            glVertex2i(0,   0);
            glTexCoord2i(0, 0);
            glVertex2i(0,   scr_height);
            glTexCoord2i(1, 0);
            glVertex2i(scr_width, scr_height);
            glTexCoord2i(1, 1);
            glVertex2i(scr_width, 0);
            glEnd();
            //glFinish();
            //SDL_GL_SwapBuffers();
        }

        while(d[current_id].df!=NULL && status!=0)
        {
            if(status==0)break; // Se ha dicho de cerrar, se cierra ya!

            ret=d[current_id].df(message,&d[current_id],0);
            if(status == 4)
            {
                message = message_saved;    // Si es una orden extra volvemos a poner la de verdad
                status = 2;
            }

            is_dirty=0;
            if(d[current_id].flag & D_DIRTY)
            {
                d[current_id].flag &= !D_DIRTY;
                is_dirty=1;
            }

            switch(ret)
            {
            case D_O_K: // Continuar como si no si nada
                break;
            case D_CLOSE: // Cerrar el dialog
                status=0;
                ret_id=current_id;
                break;
            case D_REDRAW_ALL:
            case D_REDRAW: // Dibujar todo
                status=3;
                break;
            case D_REDRAWME:
                current_id--; // Evitamos que pase al siguiente elemento
                status=4;
                message_saved=message;
                message=MSG_DRAW;
                break;
            case D_WANTFOCUS: // No lo uso
                break;
            case D_USED_CHAR: // Por ahora este tampoco
                break;
            case D_DONTWANTMOUSE:
                break;
            }
            current_id++;
        }
        if(is_dirty)
        {
            glFinish();
            SDL_GL_SwapBuffers();
        }
    }

    /* Desinicialización */
    current_id=0;
    while(d[current_id].df!=NULL)
    {
        d[current_id].df(MSG_END,&d[current_id],0);
        current_id++;
    }
    ilDeleteImages(1, &pantalla);
    restore_gl_mode();

    // Esperamos un poco para que se puedan quitar el ratón y el teclado
    SDL_Delay(50);
    clear_cola_eventos();

    return ret_id;
}
