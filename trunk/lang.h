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

#define TEXT_LIST_R_ERR 10 // Inicio ID reservado para los mensajes de error
#define TEXT_LIST_R_WRN 50 // Inicio ID reservado para los mensajes de alerta
#define TEXT_LIST_R_NFO 60 // Inicio ID reservado para los mensajes de información
#define TEXT_LIST_R_SPAWN_MENU 70 // Inicio ID reservado para el spawn_menu
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
#define app_path lista_texto[0]

/* FUNCIONES */

void lang_load(const char* lang);


#endif
