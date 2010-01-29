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

/** \file control.h
 * \brief Funciones de control del programa (Entrada de teclado y rat�n... etc
 * Este archivo contiene las funciones que se encargan de controlar el estado del programa y de entrada directa
 * del usuario (teclado, rat�n, y cosas as�).
 * \author Alfonso Arbona Gimeno
*/

#include "control.h"

void control(void)
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
	if (camera.pos_z>30000){camera.pos_z=30000;}
}


void key_handle(SDLKey key, SDLMod mod)
{
	float speed=0.5f;
	float last_x=camera.pos_x;
	float last_y=camera.pos_y;
	float last_z=camera.pos_z;
	
	if (SDL_GetModState() & KMOD_SHIFT){speed=200.0f;}
    
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
		
	/*case SDLK_i: test.rot.x++;
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
	*/
    case SDLK_c:
		camera.show_grid=1;
		break;
    case SDLK_v:
		camera.show_grid=0;
		break;
		
    case SDLK_b:
		camera.show_presion=1;
		break;
    case SDLK_n:
		camera.show_presion=0;
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
	
    default:
        break;
    }
    if(coord_to_real_height(marte,camera.pos_z) - get_real_height(marte, camera.pos_x, camera.pos_y)<0){camera.pos_x=last_x;camera.pos_y=last_y;camera.pos_z=last_z;}
	control();
}

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

/* Si se mueve el rat�n mientras se est� pulsando algun bot�n del rat�n */
void mouse_move_but(int button, int x, int y)
{
	static int dont_use_this_movement;
	float last_x=camera.pos_x;
	float last_y=camera.pos_y;
	float last_z=camera.pos_z;
	int w=scr_width;
	int h=scr_height;
	int val;
	float altura_real = coord_to_real_height(marte,camera.pos_z) - get_real_height(marte, camera.pos_x, camera.pos_y);
	//debug_printf("Altura real: %f; z2real: %f; get_real: %f \n",altura_real,coord_to_real_height(marte,camera.pos_z),get_real_height(marte, camera.pos_x, camera.pos_y));
	
	if (button & SDL_BUTTON(SDL_BUTTON_RIGHT) && !(SDL_GetModState() & KMOD_SHIFT))/* Desplazamiento por la pantalla */
	{
		camera.pos_x+=x*cos(RAD(camera.yaw))*altura_real/1000.0;
		camera.pos_y+=x*sin(RAD(camera.yaw))*altura_real/1000.0;
		camera.pos_x-=y*sin(RAD(-camera.yaw))*altura_real/1000.0;
		camera.pos_y-=y*cos(RAD(-camera.yaw))*altura_real/1000.0;
	}
	else if(button & SDL_BUTTON(SDL_BUTTON_RIGHT) && (SDL_GetModState() & KMOD_SHIFT))
	{
		camera.vel_z=y*altura_real/1000.0;
	}
	
	
	if (button & SDL_BUTTON(SDL_BUTTON_MIDDLE))/* Girar pantalla */
	{
		SDL_ShowCursor(0);
		SDL_WarpMouse(scr_width/2, scr_height/2);
		if(dont_use_this_movement>4)/* Nos aseguramos que no se trata de un warp */
		{
			camera.pitch+=y/10.0f;
			camera.yaw+=x/10.0f;
		}
		if (dont_use_this_movement<=10)dont_use_this_movement++;
	}else if (dont_use_this_movement!=0){SDL_ShowCursor(1);dont_use_this_movement=0;}
	
	if (button & SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		
	}
	
    if(coord_to_real_height(marte,camera.pos_z) - get_real_height(marte, camera.pos_x, camera.pos_y)<0){camera.pos_x=last_x;camera.pos_y=last_y;camera.pos_z=last_z;}
	control();
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
			x-=last_m_x;//scr_width/2;
			y-=last_m_y;//scr_height/2;
			x*=-1;
			y*=-1;
			if (!x && !y)break;
			mouse_move_but(mstat, x, y);
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (sdl_event.button.button == SDL_BUTTON_WHEELUP)camera.vel_z=+0.2;/* No funciona si no est� aqu� */
			if (sdl_event.button.button == SDL_BUTTON_WHEELDOWN)camera.vel_z=-0.2;
			mstat = SDL_GetMouseState(&x,&y);
			x-=last_m_x;//scr_width/2;
			y-=last_m_y;//scr_height/2;
			if (!x && !y)break;
			mouse_move_but(mstat, x, y);
			break;
		}
	}
	SDL_GetMouseState(&last_m_x,&last_m_y);
	//if(window_mode==0){SDL_WarpMouse(scr_width/2, scr_height/2);}
}

void main_update(void)
{
	float last_x=camera.pos_x;
	float last_y=camera.pos_y;
	float last_z=camera.pos_z;
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
	
	if(coord_to_real_height(marte,camera.pos_z) - get_real_height(marte, camera.pos_x, camera.pos_y)<=0)
	{
		camera.pos_x=last_x;camera.pos_y=last_y;camera.pos_z=last_z;
		camera.vel_x=0;camera.vel_y=0;camera.vel_z=0;
	}
	control();
}