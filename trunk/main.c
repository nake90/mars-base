/*! \mainpage Mars Base
 *
 * \section intro_sec Introducción
 *
 * Mars-Base (nombre no definitivo) es un juego que estamos desarrollando que consistirá en diseñar, construir y mantener una colonia en Marte. Siendo en todo momento lo más fieles a la realidad en los aspectos químicos, físicos y topográficos.
 * Para crear la base se contará con módulos prefabricados (pasillos, puertas, habitaciones, biodomes,...) que se podrán ensamblar entre sí permitiendo bases de estructura diseñada por el jugador. A nivel más de interiores, se deberá diseñar el sistema de circuitos eléctricos (sensores, actuadores y suministro eléctrico) y de fluidos (aire, agua,...), aunque supongo que se podrán usar elementos prefabricados.
 * El juego está siendo desarrollado usando openGL, SDL y devIL. Por ahora tan solo puede mostrar el mapa tridimensional marciano (Valles Marineris) y cargar objetos, pero está siendo desarrollado con intensidad. 
 *
 * \section copyright Copyright
 * 
 *	mars_base - Design, build and admin your own base on Mars
 *  Copyright (C) 2009  Alfonso Arbona Gimeno (nake90@terra.es). All rights reserved.
 *
 *	MIT LICENSE
 *	
 *	Permission is hereby granted, free of charge, to any
 *	person obtaining a copy of this software and associated
 *	documentation files (the "Software"), to deal in the
 *	Software without restriction, including without limitation
 *	the rights to use, copy, modify, merge, publish,
 *	distribute, sublicense, and/or sell copies of the
 *	Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice
 *	shall be included in all copies or substantial portions of
 *	the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
 *	KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *	WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 *	PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
 *	OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *	OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 *	OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 *	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *  
 *  If you use any part of this code you must give me (Alfonso Arbona Gimeno) credit.
 *  If you plan to use any part of this code on a comercial game please email me at:
	   	   nake90@terra.es
*/

/** \file main.c
 * \brief Archivo principal del Mars-Base
 * Este archivo se encarga de la inicialización del juego y contiene el main central.
 * \author Alfonso Arbona Gimeno
*/
#include "mars_base_private.h"

#include <time.h> /* Para random */

#include <GL/gl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <IL/ilut.h>

#include "shared.h"
#include "objetos.h"
#include "overlay.h"
#include "heightmap.h"
#include "materiales.h"
#include "atmosferico.h"
#include "control.h"
#include "display.h"
#include "parser.h"

t_texture fondo={{1.0f, 1.0f, 1.0f, 1.0f},{1.0f, 1.0f, 1.0f, 1.0f},{0.0f, 0.0f, 0.0f, 1.0f},{1.0},{0}};

static Uint32 next_time; /* Controla la velocidad de actualización de la pantalla */

static
Uint32 time_left(void)
{
    Uint32 now;

    now = SDL_GetTicks();
    if(next_time <= now)
        return 0;
    else
        return next_time - now;
}

static
void salir(void)
{
	/* Unload maps */
	destroy_heightmap(&marte);
	/* Unload objects */
	lista_modelos_limpiar();
	lista_base_limpiar();
	/* Unload materials */
	unload_material(&sand);
	unload_material(&fondo);
	unload_material(&sun_texture);
	/* Unload fonts */
	TTF_CloseFont(fntCourier12);
	TTF_CloseFont(fntArial12);
	/* Unload libs */
	TTF_Quit();
	SDL_Quit();
	debug_printf(lista_texto[TEXT_LIST_R_NFO + 0]);
}

int main(int argc, char *argv[])
{
	/* - INICIACIÓN VARIABLES - */
    str_cpy(app_path,argv[0]);
    str_ruta_back(app_path);
	
    int i;
    int with_motd=1;
    char lang_str[4]; // Máximo 3 carácteres y el \0
    str_cpyl(lang_str,4,"es"); // default spanish
    debug_reset();
    debug_printf("Mars_Base v." VER_STRING " (%s) - by nake\n\n",app_path);
    camera.show_grid=0;
    camera.show_presion=0;
    window_mode=0;
    
    scr_width=-1;
    scr_height=-1;
    scr_bpp=-1;
    scr_flags=-1;
    for(i=1; i<argc; i++)
    {
		if(argv[i]==NULL)break;
		if(str_cmp(argv[i],"-windowed")==0){scr_flags=FLAG_WINDOWED;}
		if(str_cmp(argv[i],"-fullscreen")==0){scr_flags=SDL_FULLSCREEN;}
		if(str_cmp(argv[i],"-width")==0 && i+1<argc){scr_width=(int)str2float(argv[i+1]);}
		if(str_cmp(argv[i],"-height")==0 && i+1<argc){scr_height=(int)str2float(argv[i+1]);}
		if(str_cmp(argv[i],"-lang")==0 && i+1<argc){str_cpyl(lang_str,4,argv[i+1]);}
		if(str_cmp(argv[i],"-no_motd")==0){with_motd=0;}
		//if(str_cmp(argv[i],"")==0){}
	}
    
    /* - INICIACIÓN PROGRAMA - */
    lang_load(lang_str);
    
    #ifdef WINDOWS
    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
    #endif
	if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)==-1))
	{ 
		debug_printf(TL_ERR_INIT_SDL, SDL_GetError());
		exit(-1);
	}
	
	video_init();
	atexit(salir);
	scr_init_reset(0);
	
	SDL_WM_SetCaption("mars_base", NULL);
	randomize(17.5);
	sfrand(); // no se por qué pero sale siempre <0 la primera vez ??
	if(sfrand()>0.0f)
	{
		fondo.texture[0] = ilutGLLoadImage("materials/fondo1.tga");
	}
	else
	{
		fondo.texture[0] = ilutGLLoadImage("materials/fondo2.tga");
	}
	if(!fondo.texture[0]){debug_printf(TL_ERR_BACKGROUND); config.show_fondo=0;}else{config.show_fondo=1;}
	/* - POSINICIALIZACIÓN (Carga elementos) - */
	
	
	/* Carga fuentes */
	fntCourier12=TTF_OpenFont("fonts/cour.ttf", 12);
	if (fntCourier12==NULL){debug_printf(TL_ERR_FONT,"cour.ttf"); exit(-1);}
	
	fntArial12=TTF_OpenFont("fonts/arial.ttf", 12);
	if (fntArial12==NULL){debug_printf(TL_ERR_FONT,"arial.ttf"); exit(-1);}
	
	/* Carga materiales */
	scr_init_printf(lista_texto[TEXT_LIST_R_SCR + 0]);
	
	icono_no_icon = ilutGLLoadImage("materials/no_icon.tga");
	if(!icono_no_icon)
	{
		debug_printf(TL_ERR_NOICON_TEXTURE);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		ilutGLBuildMipmaps();
	}
	
	null_texture = ilutGLLoadImage("materials/null.tga");
	if(!null_texture)
	{
		debug_printf(TL_ERR_NULL_TEXTURE);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		ilutGLBuildMipmaps();
	}
	
	i=load_material(&sand, "materials/sand_default");
	if(i){debug_printf(TL_ERR_SANDBASE_TEXTURE,i); sand.texture[0]=null_texture;}
	/*
	sand2_texture=ilutGLLoadImage("materials/metal_in_1.tga");
	if(!sand2_texture)
	{
		debug_printf("Error sandtexture2\n"); sand2_texture=null_texture;
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		ilutGLBuildMipmaps();
	}
	*/
	sun_texture.texture[0]=ilutGLLoadImage("materials/sun.tga");
	if(!sun_texture.texture[0])
	{
		debug_printf(TL_ERR_SUN_TEXTURE); sun_texture.texture[0]=null_texture;
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		ilutGLBuildMipmaps();
	}
	
	scr_init_printf (lista_texto[TEXT_LIST_R_SCR + 1]);
	camera.pitch = -25;
	camera.yaw = 0;
	camera.pos_x=0;
	camera.pos_z=10;
	camera.pos_y=-4.5;
	camera.wasd_count=0;
	camera.ghost_mode=0;
	
	scr_init_printf (lista_texto[TEXT_LIST_R_SCR + 2]);
	if((i=load_heightmap("heightmaps/marineris",&marte,sand))!=0){debug_printf(TL_ERR_LOAD_HEIGHTMAP,i);exit(-1);}
	
	scr_init_printf (lista_texto[TEXT_LIST_R_SCR + 3]);
	
	lista_objeto_base=NULL;
	lista_modelo=NULL;
	lista_objetos_base=0;
	lista_modelos=0;
	
	char buffer[256];
	str_cpyl(buffer,256,app_path);
	str_append(buffer,"models");
	if(lista_cargar_modelo_dir(buffer)!=0){debug_printf(TL_ERR_MODEL_DIR,buffer);}
	
	scr_init_printf (lista_texto[TEXT_LIST_R_SCR + 13]);
	glClearColor(fogColor[0],fogColor[1],fogColor[2],1.0f);
	
	
	/* - MAIN LOOP - */
	next_time = SDL_GetTicks() + TICK_INTERVAL;
	
	// MENSAJE DE INTRODUCCIÓN
	if(with_motd)
	{
		char *motd_text = malloc(sizeof(char) * (str_size(TL_MOTD1)+str_size(TL_MOTD2)+2)); // +2 para el \0 y por si acaso...
		if(motd_text!=NULL)
		{
			if(str_size(TL_MOTD1) + str_size(TL_MOTD2) > 0)
			{
				str_cpy(motd_text, TL_MOTD1);
				str_append(motd_text, TL_MOTD2);
				message_printf(motd_text);
			}
			free(motd_text);
		}
		else
		{
			debug_printf(TL_ERR_MALLOC,"motd buffer");
		}
	}
	
	
	int node_main_control_loop=1;
	while(1)
	{
		process_events();
		
		main_update();
		
		display();
		SDL_GL_SwapBuffers();
		SDL_Delay(time_left());
		next_time += TICK_INTERVAL;
		if(node_main_control_loop)
		{
			node_main_control(TICK_INTERVAL*2/1000.0);
			node_main_control_loop=0;
		}else{node_main_control_loop=1;}
		//contador++;
		//if(contador>=FPS_FRAMES){FPS=1000.0f/(float)(next_time-SDL_GetTicks()); contador=0;}
	}
	
    exit(0);
}
