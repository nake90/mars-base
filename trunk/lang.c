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

/** \file lang.c
 * \brief Código sobre las funciones de carga del texto estático
 * \author Alfonso Arbona Gimeno
*/

#include "lang.h"
#include "shared.h"
#include "parser.h"
#include <stdio.h>

static void lang_parse_special_chars(const unsigned int start, const unsigned int end)
{
	unsigned int index, dest, src;
	char buffer[TEXT_LIST_MAX_SIZE];
	for(index=start; index<=end; index++)
	{
		src=0;
		dest=0;
		while(lista_texto[index][src]!='\0')
		{
			if(lista_texto[index][src]=='\\' && lista_texto[index][src+1]!='\0')
			{
				switch(lista_texto[index][src+1])
				{
					case 'n':
						buffer[dest]='\n';
						src++;
						break;
					case 't':
						buffer[dest]='\t';
						src++;
						break;
					case '\"':
						buffer[dest]='\"';
						src++;
						break;
				}
			}
			else
			{
				buffer[dest]=lista_texto[index][src];
			}
			src++;
			dest++;
		}
		buffer[dest]='\0';
		str_cpy(lista_texto[index],buffer);
	}
}

#define _PARSE_GET(s) if((parse_opened==0) || parse_get_str(&parse, (s), lista_texto[i])==-1){str_cpy(lista_texto[i],(s"\n"));}i++
void lang_load(const char* lang)
{
	char ruta[1024];
	int parse_opened=1;
	unsigned int i=0; // Ayuda muuucho (incrementado en el define)
	sprintf(ruta,"lang/%s.txt",lang);
	t_parse parse;
	if(parse_open(&parse, ruta)!=0){parse_opened=0; debug_printf("Error, lang (%s) not found!\n",lang);} // In inglish pitinglish porque es un error al cargar la traducción de los errores xD
	
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
	_PARSE_GET("err_open");
	_PARSE_GET("err_model_not_found");					// +10
	_PARSE_GET("err_map_malloc");
	_PARSE_GET("err_map_pgm_type");
	_PARSE_GET("err_precompiled_type");
	_PARSE_GET("err_precompiled_version");
	_PARSE_GET("err_list_create_model_id");				// +15
	_PARSE_GET("err_video_init");
	_PARSE_GET("err_ttf_init");
	_PARSE_GET("err_devil_version");	
	_PARSE_GET("err_material_load_texture");
	_PARSE_GET("err_ghost_mode_already_started");		// +20
	_PARSE_GET("err_ghost_mode_model_id");
	_PARSE_GET("err_malloc");
	_PARSE_GET("err_realloc");
	_PARSE_GET("err_malloc_id");
	_PARSE_GET("err_model_load");						// +25
	_PARSE_GET("ERR");
	lang_parse_special_chars(TEXT_LIST_R_ERR, TEXT_LIST_R_ERR + 25);
	
	/* Cargamos las alertas (TEXT_LIST_R_WRN) */
	i=TEXT_LIST_R_WRN;
	_PARSE_GET("wrn_minimap");							// +0
	_PARSE_GET("wrn_precompiled_not_found");
	_PARSE_GET("wrn_ghost_mode_config_file");
	_PARSE_GET("wrn_model_load_texture_not_found");
	_PARSE_GET("wrn_model_load_icon");
	_PARSE_GET("ERR");									// +5
	lang_parse_special_chars(TEXT_LIST_R_WRN, TEXT_LIST_R_WRN + 4);
	
	/* Cargamos los mensajes de información (TEXT_LIST_R_NFO) */
	i=TEXT_LIST_R_NFO;
	_PARSE_GET("nfo_closed_ok");						// +0
	_PARSE_GET("nfo_compiled_start");
	_PARSE_GET("nfo_compiled_ok");
	_PARSE_GET("nfo_model_autoload");
	_PARSE_GET("nfo_free_base_object");
	_PARSE_GET("nfo_free_model");						// +5
	_PARSE_GET("ERR");
	lang_parse_special_chars(TEXT_LIST_R_NFO, TEXT_LIST_R_NFO + 5);
	
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
	_PARSE_GET("scr_message_load_base");
	_PARSE_GET("ERR");									// +15
	lang_parse_special_chars(TEXT_LIST_R_SCR, TEXT_LIST_R_SCR + 14);
	
	/* Cargamos los mensajes de los dialogs (TEXT_LIST_R_DLG) */
	i=TEXT_LIST_R_DLG;
	_PARSE_GET("dlg_message_printf_close");				// +0
	_PARSE_GET("dlg_spawn_dialog_title");
	_PARSE_GET("dlg_spawn_dialog_close");
	_PARSE_GET("dlg_screenshot");
	_PARSE_GET("ERR");
	lang_parse_special_chars(TEXT_LIST_R_DLG, TEXT_LIST_R_DLG + 3);
	
	/* Cargamos el texto del HUD (TEXT_LIST_R_HUD) */
	i=TEXT_LIST_R_HUD;
	_PARSE_GET("hud_to_floor");							// +0
	_PARSE_GET("hud_ghost_mode_message");
	_PARSE_GET("ERR");
	lang_parse_special_chars(TEXT_LIST_R_HUD, TEXT_LIST_R_HUD + 2);
	
	if(parse_opened==1)parse_close(&parse);
	
	FILE *file = fopen("motd.txt","r");
	char buff[1024];
	int contador=0;
	str_cpy(TL_MOTD1,"");
	str_cpy(TL_MOTD2,"");
	if(file!=NULL)
	{
		while(!feof(file))
		{
			fgets(buff,1024,file);
			contador+=str_size(buff);
			if(contador<1024)
			{
				str_append(TL_MOTD1,buff);
			}
			else if(contador<1024 * 2)
			{
				str_append(TL_MOTD2,buff);
			}
			else{debug_printf("WARNING: MOTD size too big.");break;}
		}
	}
}
