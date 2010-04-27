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

/** \file lang.h
 * \brief Header sobre las funciones de carga del texto estático
 * \author Alfonso Arbona Gimeno
*/

#ifndef LANG_H
#define LANG_H

#define TEXT_LIST_MAX_QTY 256
#define TEXT_LIST_MAX_SIZE 1024

/* CUIDADO OVERFLOWS!! */
#define TEXT_LIST_R_ERR 10 // Inicio ID reservado para los mensajes de error
#define TEXT_LIST_R_WRN 50 // Inicio ID reservado para los mensajes de alerta
#define TEXT_LIST_R_NFO 60 // Inicio ID reservado para los mensajes de información
#define TEXT_LIST_R_SPAWN_MENU 70 // Inicio ID reservado para el spawn_menu  *** No tengo tan claro que se use (tal vez cuando sea más complejo sí) ***
#define TEXT_LIST_R_SCR 95 // Inicio ID reservado para los mensajes de carga
#define TEXT_LIST_R_DLG 115 // Inicio ID reservado para los mensajes de los dialogs
#define TEXT_LIST_R_HUD 130 // Inicio ID reservado para los mensajes del HUD
#define TEXT_LIST_R_USER 150 // Inicio ID no reservado (Uso solo en funciones pequeñas)
char lista_texto[TEXT_LIST_MAX_QTY][TEXT_LIST_MAX_SIZE]; // Lista de mensajes y texto necesario para los dialogs. Además permite mods y traducciones ;)
/* Lugares reservados de la lista:
0 - 9 (10) -> Internos (app_path ...)
10 - 49 (40) -> Mensajes de error
50 - 59 (10) -> Mensajes de alerta
60 - 69 (10) -> Mensajes de información
70 - 94 (25) -> Spawn menu
95 - 114 (20) -> Carga
115 - 129 (15) -> Dialogs
130 - 149 (20) -> HUD
150+ -> USER
*/

/* DEFINES ÚTILES */
#define app_path					lista_texto[0]
//
#define TL_MOTD1					lista_texto[2]
#define TL_MOTD2					lista_texto[3]
#define TL_ERR_INIT_SDL				lista_texto[TEXT_LIST_R_ERR + 0]
#define TL_ERR_BACKGROUND			lista_texto[TEXT_LIST_R_ERR + 1]
#define TL_ERR_FONT					lista_texto[TEXT_LIST_R_ERR + 2]
#define TL_ERR_NULL_TEXTURE			lista_texto[TEXT_LIST_R_ERR + 3]
#define TL_ERR_NOICON_TEXTURE		lista_texto[TEXT_LIST_R_ERR + 4]
#define TL_ERR_SANDBASE_TEXTURE		lista_texto[TEXT_LIST_R_ERR + 5]
#define TL_ERR_SUN_TEXTURE			lista_texto[TEXT_LIST_R_ERR + 6]
#define TL_ERR_LOAD_HEIGHTMAP		lista_texto[TEXT_LIST_R_ERR + 7]
#define TL_ERR_MODEL_DIR			lista_texto[TEXT_LIST_R_ERR + 8]
#define TL_ERR_GEN_OPEN				lista_texto[TEXT_LIST_R_ERR + 9]
#define TL_ERR_MODEL_NOT_FOUND		lista_texto[TEXT_LIST_R_ERR + 10]
#define TL_ERR_MAP_MALLOC			lista_texto[TEXT_LIST_R_ERR + 11]
#define TL_ERR_MAP_PGM_TYPE			lista_texto[TEXT_LIST_R_ERR + 12]
#define TL_ERR_PRECOMP_TYPE			lista_texto[TEXT_LIST_R_ERR + 13]
#define TL_ERR_PRECOMP_VER			lista_texto[TEXT_LIST_R_ERR + 14]
#define TL_ERR_LIST_CREATE_MODEL_ID	lista_texto[TEXT_LIST_R_ERR + 15]
#define TL_ERR_VIDEO_INIT			lista_texto[TEXT_LIST_R_ERR + 16]
#define TL_ERR_TTF_INIT				lista_texto[TEXT_LIST_R_ERR + 17]
#define TL_ERR_DEVIL_VER			lista_texto[TEXT_LIST_R_ERR + 18]
#define TL_ERR_MAT_LOAD_TEXTURE		lista_texto[TEXT_LIST_R_ERR + 19]
#define TL_ERR_GHOST_STARTED		lista_texto[TEXT_LIST_R_ERR + 20]
#define TL_ERR_GHOST_MODEL_ID		lista_texto[TEXT_LIST_R_ERR + 21]
#define TL_ERR_MALLOC				lista_texto[TEXT_LIST_R_ERR + 22]
#define TL_ERR_REALLOC				lista_texto[TEXT_LIST_R_ERR + 23]
#define TL_ERR_MALLOC_ID			lista_texto[TEXT_LIST_R_ERR + 24]
#define TL_ERR_MODEL_LOAD			lista_texto[TEXT_LIST_R_ERR + 25]


/* FUNCIONES */

void lang_load(const char* lang);


#endif
