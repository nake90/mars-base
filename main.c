/*! \mainpage Mars Base
 *
 * \section intro_sec Introducción
 *
 * Mars-Base (nombre no definitivo) es un juego que estamos desarrollando que consistirá en diseñar, construir y mantener una colonia en Marte. Siendo en todo momento lo más fieles a la realidad en los aspectos químicos, físicos y topográficos.
 * Para crear la base se contará con módulos prefabricados (pasillos, puertas, habitaciones, biodomes,...) que se podrán ensamblar entre sí permitiendo bases de estructura diseñada por el jugador. A nivel más de interiores, se deberá diseñar el sistema de circuitos eléctricos (sensores, actuadores y suministro eléctrico) y de fluidos (aire, agua,...), aunque supongo que se podrán usar elementos prefabricados.
 * El juego está siendo desarrollado usando openGLUT y devIL. Por ahora tan solo puede mostrar el mapa tridimensional marciano (Valles Marineris) y cargar objetos simples, pero está siendo desarrollado con intensidad. 
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

#include <stdio.h> /* Archivos */
#include <stdarg.h> /* Argumentos variables (Como printf()) */
#include <math.h> /* Math */
#include <time.h> /* Para random */
#include <pthread.h> /* Multithreading */

/* Librerías añadidas a las carpetas 'include' y 'lib' del proyecto */
//#include "GL/openglut.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
//#include <SDL/SDL_thread.h>
#include <SDL_ttf.h>
#include <IL/ilut.h>

#include "shared.h"
#include "objetos.h"
#include "overlay.h"
#include "heightmap.h"
#include "materiales.h"
#include "atmosferico.h"


SDL_Surface *screen;
const SDL_VideoInfo* scr_info = NULL;
int scr_width = 0;
int scr_height = 0;
int scr_bpp = 0;
int scr_flags = 0;

SDL_Event sdl_event;

static Uint32 next_time; /* Controla la velocidad de actualización de la pantalla */
static float FPS;
#define FPS_FRAMES 10

int show_grid;
int show_presion;


t_model test_data;
t_model test2_data;
t_obj_base test;
t_obj_base test2;

/* Niebla y atmósfera */
GLfloat fogColor[4]= {0.81f, 0.64f, 0.61f, 1.0f}; /*!< Color de la niebla */
GLfloat fogRange[2]= {25000.0f, 50000.0f}; /*!< Distancia mínima de la niebla y distancia máxima visible en metros, pregunta interesante... distancia máxima 50Km?? */

/* TEXTURAS BÁSICAS */
t_texture sand;
/*					AMBIENT					DIFFUSE					SPECULAR				POSITION			HORA   TEXTURES*/
t_sun sun={{0.5f, 0.5f, 0.5f, 1.0f},{1.0f, 1.0f, 1.0f, 1.0f},{1.0f, 1.0f, 1.0f, 1.0f},{10000.0f, 20000.0f, 30000.0f},{12.0f},{0,0}};
/*							AMBIENT						DIFFUSE						SPECULAR		SHININESS TEXTURE */
t_texture sun_texture={{1.0f, 1.0f, 1.0f, 1.0f},{1.0f, 1.0f, 1.0f, 1.0f},{0.0f, 0.0f, 0.0f, 1.0f},{1.0},{0}};



int window_mode; /* 0->Normal, 1->Minimizado */

Uint32 time_left(void)
{
    Uint32 now;

    now = SDL_GetTicks();
    if(next_time <= now)
        return 0;
    else
        return next_time - now;
}


static void resize_window(int width, int height)
{
	const float ar = (float) width / (float) height;
	/* NORMAL */
	glViewport(0, 0, width, height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,fogRange[1]+100.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void draw_grid(t_heightmap map, int x, int y)
{
	if (x<0 || y<0 || x>=map.tam_x || y>=map.tam_y)return;
	int i;
	int half_x, half_y;
	half_x = map.tam_x/2;
	half_y = map.tam_y/2;
	
	glDisable(GL_TEXTURE_2D);
	glLineWidth(1.0f);
	for (i=0; i<=map.scale; i++)
	{
		if(i!=0){glColor3d(0.7f,0.7f,1.0f);}else{glColor3d(0.0f,0.0f,1.0f);}
		glBegin(GL_LINES);
			/* Horizontal */
			glVertex3f( (map.tam_x-x-1 - half_x)*map.scale-map.ini_x +i, (y+1 - half_y)*map.scale-map.ini_y, get_real_height(map,(map.tam_x-x-1 - half_x)*map.scale-map.ini_x +i, (y+1 - half_y)*map.scale-map.ini_y));
			glVertex3f( (map.tam_x-x-1 - half_x)*map.scale-map.ini_x +i, (y+2 - half_y)*map.scale-map.ini_y, get_real_height(map,(map.tam_x-x-1 - half_x)*map.scale-map.ini_x +i, (y+2 - half_y)*map.scale-map.ini_y));
			/* Vertical */
			glVertex3f( (map.tam_x-x-0 - half_x)*map.scale-map.ini_x, (y+1  - half_y)*map.scale-map.ini_y+i, 0.0f);
			glVertex3f( (map.tam_x-x-1 - half_x)*map.scale-map.ini_x, (y+1  - half_y)*map.scale-map.ini_y+i, 0.0f);
		glEnd();
	}
}

void display(void)
{
	
	int i;
    int j;
    VECTOR v;
    
    float r,g,b;
    float r2,b2;
    float r3,b3;
    float r4,b4;
	
	glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glRotatef(-camera.pitch, 1.0f,0,0);
    glRotatef(-camera.yaw, 0,0,1.0f);
    glRotatef(-camera.roll, sin(RAD(-camera.yaw)),cos(RAD(-camera.yaw)),-sin(RAD(camera.pitch)));
	/* Dibujos estáticos (Sky) */
	
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	draw_fixsprite(sun.position[0],sun.position[1],sun.position[2]+SUN_SIZE,sun_texture,SUN_SIZE);
	//glBlendFunc(GL_ONE,GL_ZERO);
	//glDisable(GL_BLEND);
	
	/* Dibujo normal */
	glTranslatef(-camera.pos_x,-camera.pos_y,-camera.pos_z);
    glEnable(GL_LIGHTING);
    glEnable(GL_FOG);
	
	
    glLightfv(GL_LIGHT0, GL_POSITION, sun.position);
    
	glCallList(marte.list);/* Dibujamos el terreno */
	
	
	/* Dibujamos los extras del terreno */
	
	/* - Display Casillas - FIXME!!!!! (casillas solo de la zona actual, de 1x1m^2) */
   	/*if (show_grid)
	{*/
		//draw_grid(marte, (int)(-((camera.pos_x+marte.ini_x)/marte.scale) + marte.tam_x/2), (int)(((camera.pos_y+marte.ini_y)/marte.scale) + marte.tam_y/2)-1);
		//hud_printf (1, 9, "(%i,%i)",(int)(((camera.pos_x+marte.ini_x)/marte.scale) + marte.tam_x/2), (int)(-((camera.pos_y+marte.ini_y)/marte.scale) + marte.tam_y/2)-1);
	/*}*/
	glColor3d(1.0,1.0,1.0);
	
	if (show_grid) /* Ahora muestra la cuadrícula */
	{
		glCallList(marte.list+1);
	}
	if (show_presion) /* Ahora muestra las normales */
	{
		glCallList(marte.list+2);
	}
	glColor3d(1.0,1.0,1.0);
	
	/* Dibujamos los objetos */
	object_draw_l(&(test));
	object_draw_l(&(test2));
	
	
	
    glDisable(GL_LIGHTING);
    
	/* Dibujamos el HUD */
	draw_HUD();
	hud_printf (12, 10*12, "Altura hasta el suelo: %f",coord_to_real_height(marte,camera.pos_z) - get_real_height(marte, camera.pos_x, camera.pos_y));
	hud_printf (12, 11*12, "FPS: %3.2f",FPS);
	
	
	//glutSwapBuffers();
	SDL_GL_SwapBuffers();
}

static void control (void)
{
	if(camera.pitch<0){camera.pitch=0;}
	if(camera.pitch>180){camera.pitch=180;}
	if(camera.yaw<0){camera.yaw+=360;}
	if(camera.yaw>=360){camera.yaw-=360;}
	if(camera.roll<0){camera.roll+=360;}
	if(camera.roll>=360){camera.roll-=360;}
	if (camera.pos_x<(-marte.tam_x/2)*marte.scale -marte.ini_x){camera.pos_x=(-marte.tam_x/2)*marte.scale -marte.ini_x;}
	if (camera.pos_x>(+marte.tam_x/2)*marte.scale -marte.ini_x -marte.scale*2){camera.pos_x=(+marte.tam_x/2)*marte.scale -marte.ini_x -marte.scale*2;}
	if (camera.pos_y<(-marte.tam_y/2)*marte.scale -marte.ini_y){camera.pos_y=(-marte.tam_y/2)*marte.scale -marte.ini_y;}
	if (camera.pos_y>(+marte.tam_y/2)*marte.scale -marte.ini_y -marte.scale){camera.pos_y=(+marte.tam_y/2)*marte.scale -marte.ini_y -marte.scale;}
}


static
void key_handle(SDLKey key, SDLMod mod)
{
	float speed=0.5f;
	float last_x=camera.pos_x;
	float last_y=camera.pos_y;
	float last_z=camera.pos_z;
	if (mod&KMOD_SHIFT){speed=200.0f;}
    
	VECTOR vec;
	float presion;
	switch (key)
    {
    case SDLK_ESCAPE: 
		exit(0);
		break;
	case SDLK_w: camera.pitch++;
		break;
    case SDLK_s: camera.pitch--;
		break;
		
    case SDLK_e: camera.roll++;
		break;
    case SDLK_q: camera.roll--;
		break;
		
    case SDLK_a: camera.yaw++;
		break;
    case SDLK_d: camera.yaw--;
		break;
		
	case SDLK_i: test.rot.x++;
		break;
    case SDLK_k: test.rot.x--;
		break;
		
    case SDLK_o: test.rot.z++;
		break;
    case SDLK_u: test.rot.z--;
		break;
		
    case SDLK_j: test.rot.y++;
		break;
    case SDLK_l: test.rot.y--;
		break;
	
    case SDLK_c:
		show_grid=1;
		break;
    case SDLK_v:
		show_grid=0;
		break;
		
    case SDLK_b:
		show_presion=1;
		break;
    case SDLK_n:
		show_presion=0;
		break;
		
    case SDLK_t: /* Funciones de test */
		
		break;
	
	case SDLK_UP:
		camera.vel_x+=speed*sin(RAD(-camera.yaw));
		camera.vel_y+=speed*cos(RAD(-camera.yaw));
		camera.wasd_count++;
		break;
    case SDLK_DOWN:
		camera.vel_x+=-speed*sin(RAD(-camera.yaw));
		camera.vel_y+=-speed*cos(RAD(-camera.yaw));
		camera.wasd_count++;
		break;
		
    case SDLK_RIGHT:
		camera.vel_x+=speed*cos(RAD(camera.yaw));
		camera.vel_y+=speed*sin(RAD(camera.yaw));
		camera.wasd_count++;
		break;
    case SDLK_LEFT:
		camera.vel_x+=-speed*cos(RAD(camera.yaw));
		camera.vel_y+=-speed*sin(RAD(camera.yaw));
		camera.wasd_count++;
		break;
		
    case SDLK_PAGEUP: camera.pos_z+=speed;
		break;
    case SDLK_PAGEDOWN: camera.pos_z-=speed;
		break;
	
	
    default:
        break;
    }
    if(coord_to_real_height(marte,camera.pos_z) - get_real_height(marte, camera.pos_x, camera.pos_y)<0){camera.pos_x=last_x;camera.pos_y=last_y;camera.pos_z=last_z;}
	control();
}

static
void key_up_handle(SDLKey key, SDLMod mod)
{
	switch (key)
    {
	case SDLK_UP:
    case SDLK_DOWN:
    case SDLK_RIGHT:
    case SDLK_LEFT:
		camera.wasd_count--;
		break;
    default:
        break;
    }
}

static /* Si se mueve el ratón mientras se está pulsando algun botón del ratón */
void mouse_move_but(int button, int x, int y)
{
	int w=scr_width;
	int h=scr_height;
	int val;
	float altura_real = coord_to_real_height(marte,camera.pos_z) - get_real_height(marte, camera.pos_x, camera.pos_y);
	//debug_printf("Altura real: %f; z2real: %f; get_real: %f \n",altura_real,coord_to_real_height(marte,camera.pos_z),get_real_height(marte, camera.pos_x, camera.pos_y));
	
	if (button & SDL_BUTTON(SDL_BUTTON_RIGHT))/* Desplazamiento por la pantalla */
	{
		camera.pos_x+=x*cos(RAD(camera.yaw))*altura_real/1000.0;
		camera.pos_y+=x*sin(RAD(camera.yaw))*altura_real/1000.0;
		camera.pos_x-=y*sin(RAD(-camera.yaw))*altura_real/1000.0;
		camera.pos_y-=y*cos(RAD(-camera.yaw))*altura_real/1000.0;
	}
	
	/*if (b_raton[B_IZQ_RATON] && b_raton[B_DER_RATON])/* Zoom /
	{
		/* cy -> Incremento de posición del ratón /
		cy=(p_raton_last_pres[1]-y);
		camera.pos_z+=cy*altura_real/1000.0;
	}*/
	
	if (button & SDL_BUTTON(SDL_BUTTON_MIDDLE))/* Girar pantalla */
	{
		camera.pitch+=y/10.0f;
		camera.yaw+=x/10.0f;
	}
	
	if (button & SDL_BUTTON(SDL_BUTTON_WHEELUP))
	{
		camera.vel_z=+1;
	}
	if (button & SDL_BUTTON(SDL_BUTTON_WHEELDOWN))
	{
		camera.vel_z=-1;
	}
	
	control();
}


static
void salir(void)
{
	/* Unload maps */
	destroy_heightmap(&marte);
	/* Unload objects */
	model_unload(test.modelo);
	/* Unload materials */
	unload_material(&sand);
	unload_material(&sun_texture);
	/* Unload fonts */
	TTF_CloseFont(fntCourier12);
	TTF_CloseFont(fntArial12);
	/* Unload libs */
	TTF_Quit();
	SDL_Quit();
}

static
void video_init(void)
{
	scr_info = SDL_GetVideoInfo();
	
	scr_bpp = scr_info->vfmt->BitsPerPixel; /* Usamos la config del escritorio */
	
	/* PREVIO del GL*/
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	scr_flags = SDL_OPENGL | SDL_FULLSCREEN;
	
	/* Laarga lista de resoluciones posibles xD incluso para 16:9 . He usado las más comunes, fuente: http://en.wikipedia.org/wiki/Display_resolution */
	scr_width = 1680;
	scr_height = 1050;
	/* Hmmm he leido hace poco que SDL emula un modo si no lo puede hacer.. así que a la mierda la lista */
	screen = SDL_SetVideoMode(scr_width, scr_height, scr_bpp, scr_flags);
	if ( screen == NULL )
	{
		scr_width = 1440;
		scr_height = 900;
		screen = SDL_SetVideoMode(scr_width, scr_height, scr_bpp, scr_flags);
		if ( screen == NULL )
		{
			scr_width = 1280;
			scr_height = 1024;
			screen = SDL_SetVideoMode(scr_width, scr_height, scr_bpp, scr_flags);
			if ( screen == NULL )
			{
				scr_width = 1280;
				scr_height = 800;
				screen = SDL_SetVideoMode(scr_width, scr_height, scr_bpp, scr_flags);
				if ( screen == NULL )
				{
					scr_width = 1024;
					scr_height = 768;
					screen = SDL_SetVideoMode(scr_width, scr_height, scr_bpp, scr_flags);
					if ( screen == NULL )
					{
						scr_width = 800;
						scr_height = 600;
						screen = SDL_SetVideoMode(scr_width, scr_height, scr_bpp, scr_flags);
						if ( screen == NULL )
						{
							debug_printf("No se ha podido iniciar la pantalla: %s\n", SDL_GetError());
							exit(1);
						}
					}
				}
			}
		}
	}
	resize_window(scr_width,scr_height);
	
	if (TTF_Init()!=0)
	{
		debug_printf("No se ha podido iniciar el módulo de fuentes SDL_TTF: %s.\n", TTF_GetError());
		exit(-1);
	}
	
	/* Funciones por mensaje */
	atexit(salir);/*

	/* DevIL init */
	ilInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	
	/* OpenGL - Specific */
	glShadeModel(GL_SMOOTH);
	glClearColor(0.02f,0.1f,0.02f,1.0f);
	
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, 0.32f);
	glHint(GL_FOG_HINT, GL_DONT_CARE);
	glFogf(GL_FOG_START, fogRange[0]);
	glFogf(GL_FOG_END, fogRange[1]);
	glEnable(GL_FOG);
	
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
    glEnable(GL_CULL_FACE);
    //glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
	
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  sun.ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  sun.diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, sun.specular);
    glLightfv(GL_LIGHT0, GL_POSITION, sun.position);
}

void process_events(void)
{
	Uint8 mstat;
	int x,y;
	while(SDL_PollEvent(&sdl_event))
	{
		switch(sdl_event.type)
		{
		case SDL_QUIT:
			/* Handle quit requests (like Ctrl-c). */
			exit(0);
			break;
		case SDL_ACTIVEEVENT: /* No funciona bien... */
			if(sdl_event.active.state==SDL_APPACTIVE)
			{
				window_mode=(sdl_event.active.gain==0?1:0);
				if(window_mode==0){SDL_WM_GrabInput(SDL_GRAB_ON);}else{SDL_WM_GrabInput(SDL_GRAB_OFF);}
				debug_printf("Minimizado: %i\n",window_mode);
			}
			break;
		case SDL_KEYDOWN:
			key_handle(sdl_event.key.keysym.sym, sdl_event.key.keysym.mod);
			break;
		case SDL_KEYUP:
			key_up_handle(sdl_event.key.keysym.sym, sdl_event.key.keysym.mod);
			break;
		case SDL_MOUSEMOTION:
			mstat = SDL_GetMouseState(&x,&y);
			x-=scr_width/2;
			y-=scr_height/2;
			x*=-1;
			y*=-1;
			if (!x && !y)break;
			mouse_move_but(mstat, x, y);
			break;
		case SDL_MOUSEBUTTONDOWN:
			mstat = SDL_GetMouseState(&x,&y);
			x-=scr_width/2;
			y-=scr_height/2;
			if (!x && !y)break;
			mouse_move_but(mstat, x, y);
			break;
		}
	}
	if(window_mode==0){SDL_WarpMouse(scr_width/2, scr_height/2);}
}

void main_update(void)
{
	if(camera.vel_x>CAMERA_MIN_VEL || camera.vel_x<-CAMERA_MIN_VEL)
	{
		camera.pos_x += camera.vel_x; if(!camera.wasd_count)camera.vel_x/=CAMERA_VEL_DIV;
	}
	else if(camera.vel_x!=0){camera.vel_x=0;}
	if(camera.vel_y>CAMERA_MIN_VEL || camera.vel_y<-CAMERA_MIN_VEL)
	{
		camera.pos_y += camera.vel_y; if(!camera.wasd_count)camera.vel_y/=CAMERA_VEL_DIV;
	}
	else if(camera.vel_y!=0){camera.vel_y=0;}
	if(camera.vel_z>CAMERA_MIN_VEL || camera.vel_z<-CAMERA_MIN_VEL)
	{
		camera.pos_z += camera.vel_z; if(!camera.wasd_count)camera.vel_z/=CAMERA_VEL_DIV;
	}
	else if(camera.vel_z!=0){camera.vel_z=0;}
	
	control();
}

int main(int argc, char *argv[])
{
    /* - INICIACIÓN VARIABLES - */
    str_cpy(app_path,argv[0]);
    str_ruta_back(app_path);
    int i;
    debug_reset();
    debug_printf("Mars_Base v." VER_STRING " (%s) - by nake\n\n",app_path);
	tam_mapa_x = TERR_SIZE*2;
	tam_mapa_y = TERR_SIZE*2;
    show_grid=0;
    show_presion=0;
    window_mode=0;
    
    
    /* - INICIACIÓN PROGRAMA - */
    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)==-1))
	{ 
		debug_printf("No se ha podido iniciar SDL: %s.\n", SDL_GetError());
		exit(-1);
	}
	
	video_init();
	scr_init_reset(0);

	/* - POSINICIALIZACIÓN (Carga elementos) - */
	SDL_WM_SetCaption("mars_base", NULL);
	
	/* Carga fuentes */
	fntCourier12=TTF_OpenFont("fonts/cour.ttf", 12);
	if (fntCourier12==NULL){debug_printf("Error al cargar la fuente base: \"cour.ttf\"\n",i); exit(-1);}
	
	fntArial12=TTF_OpenFont("fonts/arial.ttf", 12);
	if (fntArial12==NULL){debug_printf("Error al cargar la fuente: \"arial.ttf\"\n",i); exit(-1);}
	
	/* Carga materiales */
	scr_init_printf ("Cargando materiales...");
	
	i=load_material(&sand, "materials\\sand_default");
	if(i){debug_printf("Error al cargar la textura base!, RETURN:%i\n",i); exit(-1);}
	
	sun_texture.texture[0]=ilutGLLoadImage("materials\\sun.tga");
	if(!sun_texture.texture[0]){exit(0);}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	ilutGLBuildMipmaps();
	
	
	scr_init_printf ("Iniciando variables...");
	camera.pitch = 25;
	camera.yaw = 0;
	camera.pos_x=0;
	camera.pos_z=10;
	camera.pos_y=-4.5;
	camera.wasd_count=0;
	
	scr_init_printf ("Cargando modelos...");
	test.modelo=&test_data;
	if(load_3DS(test.modelo,"models\\base\\hq_general_4_8x8\\hq_general_4_8x8.3ds")!=0){exit(1);}
	object_predraw(&(test));
	obj_setpos(test,0,0,0);
	
	test2.modelo=&test2_data;
	if(load_3DS(test2.modelo,"models\\base\\ps_general_2_2x8\\ps_general_2_2x8.3ds")!=0){exit(1);}
	object_predraw(&(test2));
	obj_setpos(test2,-8,0,0);
	
	scr_init_printf ("Cargando terreno...");
	load_heightmap("heightmaps\\marineris",&marte,sand);
	
	SDL_ShowCursor(0); /* Ocultamos el cursor */
	//SDL_EnableKeyRepeat(100, 80);
	SDL_WM_GrabInput(SDL_GRAB_ON);
	
	scr_init_printf ("Iniciado");
	glClearColor(fogColor[0],fogColor[1],fogColor[2],1.0f);
	
	
	/* - MAIN LOOP - */
	next_time = SDL_GetTicks() + TICK_INTERVAL;
	
	unsigned int contador=0;
	
	while(1)
	{
		process_events();
		
		main_update();
		
		display();
		SDL_Delay(time_left());
		next_time += TICK_INTERVAL;
		contador++;
		if(contador>=FPS_FRAMES){FPS=1000.0f/(float)(next_time-SDL_GetTicks()); contador=0;}
	}
	
    exit(0);
}
