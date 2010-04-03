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

/** \file control.h
 * \brief Funciones de control del programa (Entrada de teclado y ratón... etc
 * Este archivo contiene las funciones que se encargan de controlar el estado del programa y de entrada directa
 * del usuario (teclado, ratón, y cosas así).
 * \author Alfonso Arbona Gimeno
*/
#ifndef CONTROL_H
#define CONTROL_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include <stdio.h>
#include <math.h>
#include <time.h>

#include "heightmap.h"
#include "shared.h"

// Máxima distancia al cuadrado desde la cámara hasta el objeto al hacer los traces
#define MAX_DIST_TRACE_OBJ_SQ 1000

void control(void);
void key_handle(SDLKey key, SDLMod mod);
void key_up_handle(SDLKey key, SDLMod mod);
void mouse_move_but(int button, int x, int y);
void mouse_static_but(int button, int x, int y);
void process_events(void);
void main_update(void);

int get_traced_object(VECTOR pos, VECTOR dir);

int last_m_x,last_m_y;
SDL_Event sdl_event;
int window_mode; /* 0->Normal, 1->Minimizado */

#endif
