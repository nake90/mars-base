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

/** \file lang.c
 * \brief Código sobre las funciones de carga del texto estático
 * \author Alfonso Arbona Gimeno
*/

#include "lang.h"
#include "shared.h"
#include "parser.h"

#define _PARSE_GET(s) if(parse_get_str(&parse, (s), lista_texto[i])==-1){str_cpy(lista_texto[i],(s"\n"));}i++
void lang_load(const char* lang)
{
	char ruta[1024];
	unsigned int i=0; // Ayuda muuucho
	sprintf(ruta,"lang/%s.txt",lang);
	t_parse parse;
	if(parse_open(&parse, ruta)!=0){debug_printf("Error, lang (%s) not found!\n",lang);} // In inglish pitinglish porque es un error al cargar la traducción de los errores xD
	
	/* Cargamos los errores (TEXT_LIST_R_ERR) */
	i=TEXT_LIST_R_ERR;
	_PARSE_GET("err_init_sdl");							// +0
	_PARSE_GET("err_background");
	_PARSE_GET("err_font");
	_PARSE_GET("err_null_texture");
	_PARSE_GET("err_noicon_texture");
	_PARSE_GET("err_sand_base_texture");				// +5
	_PARSE_GET("err_sun_texture");
	_PARSE_GET("err_load_heightmap");
	_PARSE_GET("err_model_dir");
	_PARSE_GET("err_open_map_pgm");
	_PARSE_GET("err_open_map_info");					// +10
	_PARSE_GET("err_map_malloc_data");
	_PARSE_GET("err_map_malloc_shadow");
	_PARSE_GET("err_map_malloc_normal");
	_PARSE_GET("err_map_pgm_type");
	_PARSE_GET("err_precompiled_type");					// +15
	_PARSE_GET("err_precompiled_version");
	_PARSE_GET("err_message_printf_malloc");
	_PARSE_GET("err_video_init");
	_PARSE_GET("err_ttf_init");
	_PARSE_GET("err_devil_version");					// +20
	_PARSE_GET("err_material_load_texture");
	_PARSE_GET("err_ghost_mode_already_started");
	_PARSE_GET("err_ghost_mode_model_id");
	_PARSE_GET("ERR");
	
	/* Cargamos las alertas (TEXT_LIST_R_WRN) */
	i=TEXT_LIST_R_WRN;
	_PARSE_GET("wrn_minimap");							// +0
	_PARSE_GET("wrn_precompiled_not_found");
	_PARSE_GET("wrn_ghost_mode_config_file");
	_PARSE_GET("ERR");
	
	/* Cargamos los mensajes de información (TEXT_LIST_R_NFO) */
	i=TEXT_LIST_R_NFO;
	_PARSE_GET("nfo_closed_ok");						// +0
	_PARSE_GET("nfo_compiled_start");
	_PARSE_GET("nfo_compiled_ok");
	_PARSE_GET("ERR");
	
	/* Cargamos los mensajes de carga (TEXT_LIST_R_SCR) */
	i=TEXT_LIST_R_SCR;
	_PARSE_GET("scr_message_load_materials");			// +0
	_PARSE_GET("scr_message_ini_variables");
	_PARSE_GET("scr_message_load_terrain");
	_PARSE_GET("scr_message_load_models");
	_PARSE_GET("scr_message_precompiled_fail");
	_PARSE_GET("scr_message_load_terrain_data");		// +5
	_PARSE_GET("scr_message_precompiled_save");
	_PARSE_GET("scr_message_predraw_terrain");
	_PARSE_GET("scr_message_precompiled_data_load");
	_PARSE_GET("scr_message_map_compile");
	_PARSE_GET("scr_message_map_compile_init");			// +10
	_PARSE_GET("scr_message_map_compile_percent");
	_PARSE_GET("scr_message_map_compile_done");
	_PARSE_GET("scr_message_done");
	_PARSE_GET("ERR");
	
	/* Cargamos los mensajes de los dialogs (TEXT_LIST_R_DLG) */
	i=TEXT_LIST_R_DLG;
	_PARSE_GET("dlg_message_printf_close");				// +0
	_PARSE_GET("dlg_spawn_dialog_title");
	_PARSE_GET("dlg_spawn_dialog_close");
	_PARSE_GET("dlg_screenshot");
	_PARSE_GET("ERR");
	
	/* Cargamos el texto del HUD (TEXT_LIST_R_HUD) */
	i=TEXT_LIST_R_HUD;
	_PARSE_GET("hud_to_floor");							// +0
	_PARSE_GET("hud_ghost_mode_message");
	_PARSE_GET("ERR");
	
	parse_close(&parse);
}
