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
 * mars_base - Design, build and maintain your own base on Mars
 * Copyright (C) 2009  Alfonso Arbona Gimeno (nake90@terra.es). All rights reserved.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * If you use any part of this code you must give me (Alfonso Arbona Gimeno) credit.
 * If you plan to use any part of this code on a comercial game please email me at:
 *    	   nake90@terra.es
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
	debug_printf("\nMars-base cerrado correctamente\n");
}

int main(int argc, char *argv[])
{
	/* - INICIACIÓN VARIABLES - */
    str_cpy(app_path,argv[0]);
    str_ruta_back(app_path);
    int i;
    debug_reset();
    debug_printf("Mars_Base v." VER_STRING " (%s) - by nake\n\n",app_path);
    camera.show_grid=0;
    camera.show_presion=0;
    window_mode=0;
    
    
    /* - INICIACIÓN PROGRAMA - */
    #ifdef WINDOWS
    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
    #endif
	if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)==-1))
	{ 
		debug_printf("No se ha podido iniciar SDL: %s.\n", SDL_GetError());
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
	if(!fondo.texture[0]){debug_printf("No se ha encontrado la textura de fondo.\n",i); config.show_fondo=0;}else{config.show_fondo=1;}
	/* - POSINICIALIZACIÓN (Carga elementos) - */
	
	
	/* Carga fuentes */
	fntCourier12=TTF_OpenFont("fonts/cour.ttf", 12);
	if (fntCourier12==NULL){debug_printf("Error al cargar la fuente base: \"cour.ttf\"\n",i); exit(-1);}
	
	fntArial12=TTF_OpenFont("fonts/arial.ttf", 12);
	if (fntArial12==NULL){debug_printf("Error al cargar la fuente: \"arial.ttf\"\n",i); exit(-1);}
	
	/* Carga materiales */
	scr_init_printf ("Cargando materiales...");
	
	icono_no_icon = ilutGLLoadImage("materials/no_icon.tga");
	if(!icono_no_icon)
	{
		debug_printf("Error al cargar la textura del no_icon! (Error grave pero continuando...)\n");
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		ilutGLBuildMipmaps();
	}
	
	i=load_material(&sand, "materials/sand_default");
	if(i){debug_printf("Error al cargar la textura base!, RETURN:%i\n",i); exit(-1);}
	
	sun_texture.texture[0]=ilutGLLoadImage("materials/sun.tga");
	if(!sun_texture.texture[0]){debug_printf("Error al cargar la textura del sol!\n"); exit(0);}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	ilutGLBuildMipmaps();
	
	
	scr_init_printf ("Iniciando variables...");
	camera.pitch = -25;
	camera.yaw = 0;
	camera.pos_x=0;
	camera.pos_z=10;
	camera.pos_y=-4.5;
	camera.wasd_count=0;
	camera.ghost_mode=0;
	
	scr_init_printf ("Cargando terreno...");
	load_heightmap("heightmaps/marineris",&marte,sand);
	
	scr_init_printf ("Cargando modelos...");
	
	lista_objeto_base=NULL;
	lista_modelo=NULL;
	lista_objetos_base=0;
	lista_modelos=0;
	
	int cont=1;
	int id;
	char key[256];
	char buffer[256];
	str_cpyl(buffer,256,app_path);
	str_append(buffer,"models");
	if(lista_cargar_modelo_dir(buffer)!=0){debug_printf("Error buscar los modelos en \"%s\"\n",buffer);}
	
	scr_init_printf ("Iniciado");
	glClearColor(fogColor[0],fogColor[1],fogColor[2],1.0f);
	
	
	/* - MAIN LOOP - */
	next_time = SDL_GetTicks() + TICK_INTERVAL;
	
	// MENSAJE DE INTRODUCCIÓN
	message_printf(	"Notas de versión:\n"
					"* No se crean objetos automáticamente, aprieta <ESPACIO> para ver el menú de objetos y haz click en el objeto que quieras crear.\n"
					"* He cambiado todo el código relacionado con pitch yaw y roll, si observas algo incorrecto al respecto avísame.\n"
					"* Control y creación de nodos en pruebas, deberían autoconectarse (en verde) los objetos con nodos definidos.");
	while(1)
	{
		process_events();
		
		main_update();
		
		display();
		SDL_GL_SwapBuffers();
		SDL_Delay(time_left());
		next_time += TICK_INTERVAL;
		
		//contador++;
		//if(contador>=FPS_FRAMES){FPS=1000.0f/(float)(next_time-SDL_GetTicks()); contador=0;}
	}
	
    exit(0);
}
