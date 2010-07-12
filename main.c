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
#include "entities.h"
#include "parser.h"

#ifdef WINDOWS // Para obtener el directorio actual
#include <direct.h>
//#define GetCurrentDir _getcwd
#else
#include <unistd.h>
//#define GetCurrentDir getcwd
#endif

t_texture fondo= {{1.0f, 1.0f, 1.0f, 1.0f},{1.0f, 1.0f, 1.0f, 1.0f},{0.0f, 0.0f, 0.0f, 1.0f},{1.0},{0}};

static Uint32 next_time; /* Controla la velocidad de actualización de la pantalla */
unsigned char debug_mode;

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
    entity_list_unload();
    lista_modelos_limpiar();
    lista_base_limpiar();
    /* Unload materials */
    unload_material(&sand);
    unload_material(&fondo);
    unload_material(&sun.material);
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
    getcwd(app_path,TEXT_LIST_MAX_SIZE);
    str_append(app_path,"/");
    //str_ruta_back(app_path);
    debug_mode=0;

    int i;
    int with_motd=1;
    char lang_str[4]; // Máximo 3 carácteres y el \0
    str_cpyl(lang_str,4,"es"); // default spanish
    debug_reset();
    debug_printf("Mars_Base(%s) v.%s.%i (%s) - by nake\n\n",INFO_STATUS,INFO_FULLVERSION_STRING,INFO_BUILDS_COUNT,app_path);
    camera.show_grid=0;
    camera.show_presion=0;
    window_mode=0;

    scr_width=-1;
    scr_height=-1;
    scr_bpp=-1;
    scr_flags=-1;

    // Cargamos la configuración de config.cfg
    t_parse parse;
    char lang_get[20]; // Espero que no escriban más texto que 20...
    if(parse_open(&parse,"config.cfg")==0)
    {
        i=parse_get_int(&parse, "fullscreen");
        if(i==0)
        {
            scr_flags=FLAG_WINDOWED;
        }
        if(i==1)
        {
            scr_flags=SDL_FULLSCREEN;
        }
        if((i=parse_get_int(&parse, "width"))>0)
        {
            scr_width=i;
        }
        if((i=parse_get_int(&parse, "height"))>0)
        {
            scr_height=i;
        }
        if(parse_get_int(&parse, "no_motd")==1)
        {
            with_motd=0;
        }
        if((i=parse_get_int(&parse, "debug"))>=0)
        {
            debug_mode=i;
        }
        if(parse_get_str(&parse, "lang", lang_get)!=PARSE_NOT_FOUND)
        {
            str_cpyl(lang_str,4,lang_get);
        }
        parse_close(&parse);
    }

    for(i=1; i<argc; i++)
    {
        if(argv[i]==NULL)break;
        if(str_cmp(argv[i],"-windowed")==0)
        {
            scr_flags=FLAG_WINDOWED;
        }
        if(str_cmp(argv[i],"-fullscreen")==0)
        {
            scr_flags=SDL_FULLSCREEN;
        }
        if(str_cmp(argv[i],"-width")==0 && i+1<argc)
        {
            scr_width=(int)str2float(argv[i+1]);
        }
        if(str_cmp(argv[i],"-height")==0 && i+1<argc)
        {
            scr_height=(int)str2float(argv[i+1]);
        }
        if(str_cmp(argv[i],"-lang")==0 && i+1<argc)
        {
            str_cpyl(lang_str,4,argv[i+1]);
        }
        if(str_cmp(argv[i],"-no_motd")==0)
        {
            with_motd=0;
        }
        if(str_cmp(argv[i],"-debug")==0 && i+1<argc)
        {
            debug_mode=(unsigned char)str2float(argv[i+1]);
        }
        //if(str_cmp(argv[i],"")==0){}
    }
    ifdebug(DEBUG_INFO)
    {
        debug_printf("Debug level: %u\n",debug_mode);
    }

    /* - INICIACIÓN PROGRAMA - */
    ifdebug(DEBUG_INFO)
    {
        debug_printf("Loading_lang\n");
    }
    lang_load(lang_str);

#ifdef WINDOWS
    ifdebug(DEBUG_DEBUG)
    {
        debug_printf("WINDOWS\n");
    }
    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
#endif
    ifdebug(DEBUG_DEBUG)
    {
        debug_printf("SDL_INIT\n");
    }
    if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)==-1))
    {
        debug_printf(TL_ERR_INIT_SDL, SDL_GetError());
        exit(-1);
    }

    ifdebug(DEBUG_DEBUG)
    {
        debug_printf("video_init\n");
    }
    video_init();
    atexit(salir);
    scr_init_reset(0);

    SDL_WM_SetCaption("mars_base", NULL);
    randomize(17.5);
    sfrand(); // no se por qué pero sale siempre <0 la primera vez ??
    if(sfrand()>0.0f)
    {
        ifdebug(DEBUG_DEBUG)
        {
            debug_printf("fondo 1\n");
        }
        fondo.texture[0] = ilutGLLoadImage("materials/fondo1.tga");
    }
    else
    {
        ifdebug(DEBUG_DEBUG)
        {
            debug_printf("fondo 2\n");
        }
        fondo.texture[0] = ilutGLLoadImage("materials/fondo2.tga");
    }
    if(!fondo.texture[0])
    {
        debug_printf(TL_ERR_BACKGROUND);
        config.show_fondo=0;
    }
    else
    {
        config.show_fondo=1;
    }

    ifdebug(DEBUG_INFO)
    {
        debug_printf("Postinicialización\n");
    }
    /* - POSTINICIALIZACIÓN (Carga elementos) - */


    /* Carga fuentes */
    ifdebug(DEBUG_INFO)
    {
        debug_printf("loading_fonts\n");
    }
    fntCourier12=TTF_OpenFont("fonts/cour.ttf", 12);
    if (fntCourier12==NULL)
    {
        debug_printf(TL_ERR_FONT,"cour.ttf");
        exit(-1);
    }

    fntArial12=TTF_OpenFont("fonts/arial.ttf", 12);
    if (fntArial12==NULL)
    {
        debug_printf(TL_ERR_FONT,"arial.ttf");
        exit(-1);
    }

    /* Carga materiales */
    ifdebug(DEBUG_INFO)
    {
        debug_printf("loading materials\n");
    }

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
    if(i)
    {
        debug_printf(TL_ERR_SANDBASE_TEXTURE,i);
        sand.texture[0]=null_texture;
    }

    if(load_material(&sun.material, "materials/sun")!=0)
    {
        debug_printf(TL_ERR_SUN_TEXTURE);
        sun.material.texture[0]=null_texture;
    }

    scr_init_printf (lista_texto[TEXT_LIST_R_SCR + 1]);
    camera.pitch = -20.0;
    camera.yaw =  0.0;
    camera.roll = 0.0;
    camera.pos_x= 0.0;
    camera.pos_z=20.0;
    camera.pos_y=-4.5;
    camera.wasd_count=0;
    camera.ghost_mode=0;

    scr_init_printf (lista_texto[TEXT_LIST_R_SCR + 2]);
    ifdebug(DEBUG_INFO)
    {
        debug_printf("Loading heightmap\n");
    }
    if((i=load_heightmap("heightmaps/marineris",&marte,sand))!=0)
    {
        debug_printf(TL_ERR_LOAD_HEIGHTMAP,i);
        exit(-1);
    }

    scr_init_printf (lista_texto[TEXT_LIST_R_SCR + 3]);

    ifdebug(DEBUG_INFO)
    {
        debug_printf("Loading objects\n");
    }
    lista_objeto_base=NULL;
    lista_modelo=NULL;
    lista_objetos_base=0;
    lista_modelos=0;

    char buffer[256];
    str_cpyl(buffer,256,app_path);
    str_append(buffer,"models");
    ifdebug(DEBUG_DEBUG)
    {
        debug_printf("Models dir: \"%s\"\n",buffer);
    }
    if(lista_cargar_modelo_dir(buffer)!=0)
    {
        debug_printf(TL_ERR_MODEL_DIR,buffer);
    }

    scr_init_printf ("Cargando entidades");

    ifdebug(DEBUG_INFO)
    {
        debug_printf("Loading entities\n");
    }
    if(entity_init()!=0){debug_printf("Error al iniciar las entidades.\n"); exit(-20);}



    scr_init_printf (lista_texto[TEXT_LIST_R_SCR + 13]);
    glClearColor(fogColor[0],fogColor[1],fogColor[2],1.0f);


    /* - MAIN LOOP - */
    next_time = SDL_GetTicks() + TICK_INTERVAL;

    // MENSAJE DE INTRODUCCIÓN
    ifdebug(DEBUG_INFO)
    {
        debug_printf("Showing MOTD\n");
    }
    if(with_motd)
    {
        ifdebug(DEBUG_VERBOSE)
        {
            debug_printf("MOTD size: %i+%i\n",str_size(TL_MOTD1),str_size(TL_MOTD2));
        }
        char *motd_text = malloc(sizeof(char) * (str_size(TL_MOTD1)+str_size(TL_MOTD2)+2)); // +2 para el \0 y por si acaso...
        if(motd_text!=NULL)
        {
            str_cpy(motd_text, TL_MOTD1);
            if(str_size(TL_MOTD2) > 0)
            {
                str_append(motd_text, TL_MOTD2);
            }
            message_printf(motd_text);
        }
        else
        {
            debug_printf(TL_ERR_MALLOC,"motd buffer");
        }
        free(motd_text);
    }


    ifdebug(DEBUG_INFO)
    {
        debug_printf("main_loop_init\n");
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
            for(i=0; i<lista_entities; i++)
            {
            	entity_exec_update(i,TICK_INTERVAL*2/1000.0);
            }
            node_main_control_loop=0;
        }
        else
        {
            node_main_control_loop=1;
        }
        //contador++;
        //if(contador>=FPS_FRAMES){FPS=1000.0f/(float)(next_time-SDL_GetTicks()); contador=0;}
    }
    ifdebug(DEBUG_INFO)
    {
        debug_printf("main_loop_end\n");
    }

    exit(0);
}
