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
 * mostrar datos por pantalla. Contiene funciones de iniciación, de mantenimiento, de control, de
 * display, de limpieza de pantalla.
 * \author Alfonso Arbona Gimeno
*/

#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdarg.h>
#include <SDL/SDL.h>
#include "shared.h"
#include "heightmap.h"

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

void video_init(void);
void resize_window(int width, int height);
void display(void);
void draw_grid(t_heightmap map, float pos_x, float pos_y);
void message_printf(const char *fmt, ...);

#endif
