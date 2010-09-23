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

#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdarg.h>
#include <SDL/SDL.h>
#include "shared.h"

SDL_Surface *screen;
const SDL_VideoInfo* scr_info;
extern int scr_width;
extern int scr_height;
extern int scr_bpp;
extern int scr_flags;

#define FPS_FRAMES 10
float FPS;

/* TEXTURAS BÁSICAS */
t_texture sand;

// Pequeño truco, como no hay un SDL_WINDOWED (o eso creo) pues usamos este y así flags está definido
#define FLAG_WINDOWED SDL_OPENGL
void video_init(void);
void resize_window(int width, int height);
void display(void);
void draw_heightmap(void);
void message_printf(const char *fmt, ...);

#endif
