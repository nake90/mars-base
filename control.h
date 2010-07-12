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
#define MAX_DIST_CONX_SQ 50000

/* Cantidad de elementos a parte de los iconos que hay en el spawn_dialog */
#define SPAWN_DIALOG_HEADER_SIZE 3
/* Cantidad de elementos a parte de los iconos que hay en el spawn_dialog de entidades */
#define ENTITY_SPAWN_DIALOG_HEADER_SIZE 3

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

extern int last_m_x,last_m_y;
extern SDL_Event sdl_event;
int window_mode; /* 0->Normal, 1->Minimizado */

#endif
