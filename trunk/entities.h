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

/** \file entities.h
 * \brief Creación y control de objetos asociados a entidades y la ejeccución de scripts relacionados.
 * Básicamente este archivo define todos los elementos necesarios para cargar archivos LUA que definen
 * la forma en la que se comportan las distintas entidades. Estas entidades son objetos con elementos
 * dinámicos como generadores, puertas, ...
 * \author Alfonso Arbona Gimeno
*/


#ifndef __ENTITIES_H__
#define __ENTITIES_H__

#include "shared.h"
#include "objetos.h"

#define ENTITY_NAME_MAXLEN 256


extern int lista_entities; /*!< Cantidad de entidades creadas en el mapa */

int entity_init(void);
void entity_execf(unsigned int id, const char* function_name);
void entity_exec_update(unsigned int id, double time_elapsed);

int entity_list_load(const char* filename, int object_id);
void entity_list_unload(void);

#endif // __ENTITIES_H__
