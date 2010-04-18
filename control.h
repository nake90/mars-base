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
#include "shared.h"

// Máxima distancia al cuadrado desde la cámara hasta el objeto al hacer los traces
#define MAX_DIST_TRACE_OBJ_SQ 5000
// Máxima distancia al cuadrado desde la cámara hasta el objeto al hacer los traces
#define MAX_DIST_CONX_SQ 50000

/* Cantidad de elementos a parte de los iconos que hay en el spawn_dialog */
#define SPAWN_DIALOG_HEADER_SIZE 3

void control(void);
void key_handle(SDLKey key, SDLMod mod);
void key_up_handle(SDLKey key, SDLMod mod);
void mouse_move_but(int button, int x, int y);
void mouse_static_but(int button, int x, int y);
void process_events(void);
void clear_cola_eventos(void);
void main_update(void);

int open_spawn_dialog(void); // Retorna el id del modelo a cargar o -1
int place_object(int id_modelo); // Ejecuta una rutina de posicionamiento del objeto, retorna 1 si se ha puesto el objeto

int get_traced_object(VECTOR pos, VECTOR dir);

int last_m_x,last_m_y;
SDL_Event sdl_event;
int window_mode; /* 0->Normal, 1->Minimizado */

#endif
