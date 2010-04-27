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
 * \brief Funciones de control del programa (Entrada de teclado y ratón... etc
 * Este archivo contiene las funciones que se encargan de controlar el estado del programa y de entrada directa
 * del usuario (teclado, ratón, y cosas así).
 * \author Alfonso Arbona Gimeno
*/

#include "control.h"
#include "atmosferico.h"
#include <math.h>
#include <SDL/SDL_ttf.h>
#include <IL/ilut.h>
#include "objetos.h"
#include "overlay.h"
#include "display.h"
#include "heightmap.h"
#include "parser.h"

extern DIALOG spawn_dialog[];

static int rotate_ghost;

void control(void)
{
	float f1;
    if(camera.pitch<-90){camera.pitch=-90;}
	if(camera.pitch>90){camera.pitch=90;}
	if(camera.yaw<0){camera.yaw+=360;}
	if(camera.yaw>=360){camera.yaw-=360;}
	if(camera.roll<0){camera.roll+=360;}
	if(camera.roll>=360){camera.roll-=360;}
	if (camera.pos_x<(-marte.tam_x/2)*marte.scale -marte.ini_x){camera.pos_x=(-marte.tam_x/2)*marte.scale -marte.ini_x;}
	if (camera.pos_x>(+marte.tam_x/2)*marte.scale -marte.ini_x -marte.scale*2){camera.pos_x=(+marte.tam_x/2)*marte.scale -marte.ini_x -marte.scale*2;}
	if (camera.pos_y<(-marte.tam_y/2)*marte.scale -marte.ini_y){camera.pos_y=(-marte.tam_y/2)*marte.scale -marte.ini_y;}
	if (camera.pos_y>(+marte.tam_y/2)*marte.scale -marte.ini_y -marte.scale){camera.pos_y=(+marte.tam_y/2)*marte.scale -marte.ini_y -marte.scale;}
	if (camera.pos_z>30000){camera.pos_z=30000;}
	if ((f1=altura_al_suelo(marte,camera.pos_x,camera.pos_y,camera.pos_z)-1.0)<0){camera.pos_z-=f1;}
}

static Uint32 next_time;
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

int place_object(int id_modelo)
{
	if(camera.ghost_mode==1){debug_printf(TL_ERR_GHOST_STARTED); return -2;}
	if (id_modelo>=lista_modelos || id_modelo<0){debug_printf(TL_ERR_GHOST_MODEL_ID); return -1;}
	camera.ghost_mode=1;
	
	// Creamos un objeto base para usar como ghost
	t_obj_base ghost;
	ghost.modelo = lista_modelo[id_modelo];
	
	/* Inicialización */
	sprintf(ghost.name,"GHOST %s",lista_modelo[id_modelo]->name);
	ghost.pos.x=0;
	ghost.pos.y=0;
	ghost.pos.z=0;
	ghost.selec = 0;
	ghost.rot.x=0;
	ghost.rot.y=0;
	ghost.rot.z=0;
	ghost.sq_l=0;
	ghost.sq_r=0;
	ghost.sq_t=0;
	ghost.sq_b=0;
	ghost.conx_qty=0;
    int con;
    VECTOR vec_zero={0,0,0};
    for(con=0;con<MAX_CONX;con++)
    {
		ghost.conx_coord[con] = vec_zero;
		ghost.conx_norm[con] = vec_zero;
		ghost.conx_id[con] = -1;
	}
    ghost.node_data.volumen = 0;
    ghost.reparar = 0;
	ghost.node_data.temperatura = 0;
	
	/* Obtenemos los datos si existen */
	t_parse parse;
	char ruta[1024];
	str_cpy(ruta,"data/models/");
	str_append(ruta,lista_modelo[id_modelo]->name);
	str_append(ruta,".cfg");
	if(parse_open(&parse,ruta)==0)
	{
		ghost.sq_l = parse_get_float(&parse,"square left");
		ghost.sq_r = parse_get_float(&parse,"square right");
		ghost.sq_t = parse_get_float(&parse,"square top");
		ghost.sq_b = parse_get_float(&parse,"square bottom");
		ghost.node_data.volumen = parse_get_float(&parse,"volumen");
					
		ghost.conx_qty = parse_get_int(&parse,"conexiones");
	    for(con=0;con<ghost.conx_qty;con++)
	    {
			// Reutilizo ruta para la entrada
			sprintf(ruta,"conx%i x",con+1);
			ghost.conx_coord[con].x = parse_get_float(&parse,ruta);
			sprintf(ruta,"conx%i y",con+1);
			ghost.conx_coord[con].y = parse_get_float(&parse,ruta);
			sprintf(ruta,"conx%i z",con+1);
			ghost.conx_coord[con].z = parse_get_float(&parse,ruta);
			
			sprintf(ruta,"conx%i dx",con+1);
			ghost.conx_norm[con].x = parse_get_float(&parse,ruta);
			sprintf(ruta,"conx%i dy",con+1);
			ghost.conx_norm[con].y = parse_get_float(&parse,ruta);
			sprintf(ruta,"conx%i dz",con+1);
			ghost.conx_norm[con].z = parse_get_float(&parse,ruta);
			
			sprintf(ruta,"conx%i size",con+1);
			ghost.conx_size[con] = parse_get_float(&parse,ruta);
		}
		
		parse_close(&parse);
	}
	else
	{
		debug_printf(lista_texto[TEXT_LIST_R_WRN + 2],lista_modelo[id_modelo]->name,ruta);
	}
	
	// Ahora está cargado por lang
	//str_cpyl(lista_texto[TEXT_LIST_R_USER + 0],TEXT_LIST_MAX_SIZE, "Haz click donde quieras el objeto, pulsa ESC para cancelar.");
	
	VECTOR pos = {camera.pos_x,camera.pos_y,camera.pos_z};
	VECTOR dir = v_from_ang(RAD(camera.pitch-90), RAD(camera.yaw));
	VECTOR coord = {0,0,0};
	/* DIALOG: {(*df), x, y, w, h, fg, bg, key, flag, d1, d2, *dp, *dp2, *dp3} */
	DIALOG mensaje = {d_label_proc, 10,200,0,0,{255,255,255,255},{0,0,0,128},0,0,0,0,lista_texto[TEXT_LIST_R_HUD + 1],fntArial12,NULL};
	// Entramos en un bucle personal interno hasta que se hace click con el ratón o se aprieta ESC
	next_time = SDL_GetTicks() + TICK_INTERVAL;
	
	int o_id, oc_id, pc_id; // Otro_ID, Otro_Conexión_ID, Propio_Conexión_ID
	VECTOR v1, v2;
	VECTOR vn1, vn2; // Vectores normales rotados
	VECTOR vzero = {0,0,0};
	int lista_conexiones[MAX_CONX]; // Usado para saber luego a qué conexión se debe poner el del otro objeto
	while(camera.ghost_mode==1)
	{
		rotate_ghost=0;
		
		process_events();
		main_update();
		
		pos.x=camera.pos_x; pos.y=camera.pos_y; pos.z=camera.pos_z;
		dir=v_from_ang(RAD(camera.pitch), RAD(camera.yaw));
		get_traced_coord(pos, dir, &coord); // Ponerlo en un if == 0!!!
		ghost.pos.x=nround(coord.x);
		ghost.pos.y=nround(coord.y);
		ghost.pos.z=nround(coord.z);
		
		ghost.rot.y += rotate_ghost;
		
		
		display();
		
		/* Control de las conexiones */
		glPointSize(8);
		for(pc_id=0; pc_id<ghost.conx_qty; pc_id++)
		{
			ghost.conx_id[pc_id]=-1;lista_conexiones[pc_id]=-1;
			v1=vrotate(ghost.conx_coord[pc_id],RAD(ghost.rot.x),RAD(-ghost.rot.y),RAD(ghost.rot.z));
			v1=vadd(v1,ghost.pos);
			
			glColor3f(1.0,0.0,0.0);
			glBegin(GL_POINTS);
				glVertex3f(v1.x,v1.y,v1.z + 1);
			glEnd();
		}
		for(o_id=0; o_id<lista_objetos_base; o_id++)
		{
			if(o_id!=id_modelo && vdist_sq(ghost.pos,lista_objeto_base[o_id]->pos)< MAX_DIST_CONX_SQ) // Si no somos nosotros mismos y no está demasiado lejos
			{
				for(pc_id=0; pc_id<ghost.conx_qty; pc_id++) // Para cada conexión propia
				{
					for(oc_id=0; oc_id<lista_objeto_base[o_id]->conx_qty; oc_id++) // Para cada conexión del otro objeto
					{
						if(nabs(ghost.conx_size[pc_id] - lista_objeto_base[o_id]->conx_size[oc_id])<=0.1) // Deben de ser del mismo tamaño
						{
							v1=vrotate(ghost.conx_coord[pc_id],RAD(ghost.rot.x),RAD(-ghost.rot.y),RAD(ghost.rot.z));
							v2=vrotate(lista_objeto_base[o_id]->conx_coord[oc_id],RAD(lista_objeto_base[o_id]->rot.x),RAD(-lista_objeto_base[o_id]->rot.y),RAD(lista_objeto_base[o_id]->rot.z));
							
							v1=vadd(v1,ghost.pos);
							v2=vadd(v2,lista_objeto_base[o_id]->pos);
							
							vn1=vrotate(ghost.conx_norm[pc_id],RAD(ghost.rot.x),RAD(-ghost.rot.y),RAD(ghost.rot.z));
							vn2=vrotate(lista_objeto_base[o_id]->conx_norm[oc_id],RAD(lista_objeto_base[o_id]->rot.x),RAD(-lista_objeto_base[o_id]->rot.y),RAD(lista_objeto_base[o_id]->rot.z));
							
							vn2=vsub(vzero,vn2); // El vector debe ser opuesto
							
							if(vdist_sq(v1,v2)<=1 && vdist_sq(vn1,vn2)<=1)
							{	// Si está en al misma coordenada y tienen direcciones opuestas
								ghost.conx_id[pc_id]=o_id;
								ghost.conx_node_id[pc_id]=oc_id;
								lista_conexiones[pc_id]=oc_id; // Esto se puede simpificar usando conx_node_id (creado más tarde)
								// La conexión del otro se aplica solo al crear el objeto!
								break; // No seguimos buscando conexiones con este objeto
							}
						}
					}
				}
			}
		}
		
		draw_element(mensaje);
		glColor4f(1.0,1.0,1.0,0.4f); // Cambiar color dependiendo de OK/MAL
		object_draw_l(&ghost);
		
		SDL_GL_SwapBuffers();
		
		SDL_Delay(time_left());
		next_time += TICK_INTERVAL;
		
	}
	if(camera.ghost_mode==0){return 0;}
	
	if(lista_base_crear_elemento(id_modelo)!=0){return -10;}
	lista_objeto_base[lista_objetos_base-1]->pos = ghost.pos;
	lista_objeto_base[lista_objetos_base-1]->rot = ghost.rot;
	lista_objeto_base[lista_objetos_base-1]->sq_l = ghost.sq_l;
	lista_objeto_base[lista_objetos_base-1]->sq_r = ghost.sq_r;
	lista_objeto_base[lista_objetos_base-1]->sq_t = ghost.sq_t;
	lista_objeto_base[lista_objetos_base-1]->sq_b = ghost.sq_b;
	lista_objeto_base[lista_objetos_base-1]->node_data.volumen = ghost.node_data.volumen;
	lista_objeto_base[lista_objetos_base-1]->reparar = ghost.reparar;
	
	// Inicialmente los gases y la presión es la de la atmósfera
	lista_objeto_base[lista_objetos_base-1]->node_data.temperatura = datos_atmosfera.temperatura;
	lista_objeto_base[lista_objetos_base-1]->node_data.gases.CO2 = datos_atmosfera.gases.CO2 * ghost.node_data.volumen;
	lista_objeto_base[lista_objetos_base-1]->node_data.gases.N2 = datos_atmosfera.gases.N2 * ghost.node_data.volumen;
	lista_objeto_base[lista_objetos_base-1]->node_data.gases.Ar = datos_atmosfera.gases.Ar * ghost.node_data.volumen;
	lista_objeto_base[lista_objetos_base-1]->node_data.gases.O2 = datos_atmosfera.gases.O2 * ghost.node_data.volumen;
	lista_objeto_base[lista_objetos_base-1]->node_data.gases.CO = datos_atmosfera.gases.CO * ghost.node_data.volumen;
	lista_objeto_base[lista_objetos_base-1]->node_data.gases.H2O = datos_atmosfera.gases.H2O * ghost.node_data.volumen;
	
	lista_objeto_base[lista_objetos_base-1]->conx_qty = ghost.conx_qty;
	for(con=0; con<ghost.conx_qty; con++)
	{
		lista_objeto_base[lista_objetos_base-1]->conx_coord[con] = ghost.conx_coord[con];
		lista_objeto_base[lista_objetos_base-1]->conx_norm[con] = ghost.conx_norm[con];
		lista_objeto_base[lista_objetos_base-1]->conx_id[con] = ghost.conx_id[con];
		lista_objeto_base[lista_objetos_base-1]->conx_size[con] = ghost.conx_size[con];
		lista_objeto_base[lista_objetos_base-1]->conx_node_id[con] = ghost.conx_node_id[con];
		if(ghost.conx_id[con]!=-1) // Asignamos al otro
		{
			lista_objeto_base[ghost.conx_id[con]]->conx_id[lista_conexiones[con]]=lista_objetos_base-1;
			lista_objeto_base[ghost.conx_id[con]]->conx_node_id[lista_conexiones[con]] = con;
		}
	}
	
	return 1;
}

#define sdh_size SPAWN_DIALOG_HEADER_SIZE
int open_spawn_dialog(void)
{
	camera.ghost_mode=0;
	/* DIALOG: {(*df), x, y, w, h, fg, bg, key, flag, d1, d2, *dp, *dp2, *dp3} */
	
	/* Inicialización del título y tal (Cuando pueda lo haré usando lua o algún otro tipo de script) */
	
	/* Ventana */
	spawn_dialog[0].df = d_box_proc;
	spawn_dialog[0].x = scr_width/2 - 621/2;
	spawn_dialog[0].y = scr_height/2 - (95 + 95 * nceil(lista_modelos/5.0))/2;
	spawn_dialog[0].w = 621;
	spawn_dialog[0].h = 95 + 95 * nceil(lista_modelos/5.0);
	spawn_dialog[0].fg.r = 0;
	spawn_dialog[0].fg.g = 0;
	spawn_dialog[0].fg.b = 0;
	spawn_dialog[0].fg.a = 255;
	spawn_dialog[0].bg.r = 40;
	spawn_dialog[0].bg.g = 128;
	spawn_dialog[0].bg.b = 40;
	spawn_dialog[0].bg.a = 200;
	spawn_dialog[0].key = 0;
	spawn_dialog[0].flag = 0;
	spawn_dialog[0].d1 = 0;
	spawn_dialog[0].d2 = 0;
	spawn_dialog[0].dp = NULL;
	spawn_dialog[0].dp2 = NULL;
	spawn_dialog[0].dp3 = NULL;
	
	/* Título */
	//str_cpyl(lista_texto[TEXT_LIST_R_SPAWN_MENU + 0],TEXT_LIST_MAX_SIZE, "Click en el objeto que quieras crear");
	
	spawn_dialog[1].df = d_label_proc;
	spawn_dialog[1].x = spawn_dialog[0].x + 40;
	spawn_dialog[1].y = spawn_dialog[0].y + 5;
	spawn_dialog[1].w = 0;
	spawn_dialog[1].h = 0;
	spawn_dialog[1].fg.r = 255;
	spawn_dialog[1].fg.g = 255;
	spawn_dialog[1].fg.b = 255;
	spawn_dialog[1].fg.a = 255;
	spawn_dialog[1].bg.r = 0;
	spawn_dialog[1].bg.g = 0;
	spawn_dialog[1].bg.b = 0;
	spawn_dialog[1].bg.a = 0;
	spawn_dialog[1].key = 0;
	spawn_dialog[1].flag = 0;
	spawn_dialog[1].d1 = 0;
	spawn_dialog[1].d2 = 0;
	spawn_dialog[1].dp = lista_texto[TEXT_LIST_R_DLG + 1];
	spawn_dialog[1].dp2 = fntArial12;
	spawn_dialog[1].dp3 = NULL;
	
	/* Botón de cancelar */
	//str_cpyl(lista_texto[TEXT_LIST_R_SPAWN_MENU + 1],TEXT_LIST_MAX_SIZE, "Cerrar");
	
	spawn_dialog[2].df = d_button_proc;
	spawn_dialog[2].x = spawn_dialog[0].x+20;
	spawn_dialog[2].y = spawn_dialog[0].y+spawn_dialog[0].h-40;
	spawn_dialog[2].w = 0;
	spawn_dialog[2].h = 0;
	spawn_dialog[2].fg.r = 255;
	spawn_dialog[2].fg.g = 255;
	spawn_dialog[2].fg.b = 255;
	spawn_dialog[2].fg.a = 255;
	spawn_dialog[2].bg.r = 64;
	spawn_dialog[2].bg.g = 128;
	spawn_dialog[2].bg.b = 64;
	spawn_dialog[2].bg.a = 255;
	spawn_dialog[2].key = 0;
	spawn_dialog[2].flag = 0;
	spawn_dialog[2].d1 = 0;
	spawn_dialog[2].d2 = 0;
	spawn_dialog[2].dp = lista_texto[TEXT_LIST_R_DLG + 2];
	spawn_dialog[2].dp2 = fntArial12;
	spawn_dialog[2].dp3 = NULL;
	
	
	/* Iconos */
	int i;
	int id=0;
	
	const int inix = 40 ,iniy = 30;
	
	int x=inix,y=iniy;
	for(i=0;i<lista_modelos*2;i++)
	{
		spawn_dialog[sdh_size+i].df = d_icon_proc;
		spawn_dialog[sdh_size+i].x = spawn_dialog[0].x + x;
		spawn_dialog[sdh_size+i].y = spawn_dialog[0].y + y;
		spawn_dialog[sdh_size+i].w = 80;
		spawn_dialog[sdh_size+i].h = 80;
		spawn_dialog[sdh_size+i].fg.r = 128;
		spawn_dialog[sdh_size+i].fg.g = 128;
		spawn_dialog[sdh_size+i].fg.b = 128;
		spawn_dialog[sdh_size+i].fg.a = 255;
		spawn_dialog[sdh_size+i].bg.r = 255;
		spawn_dialog[sdh_size+i].bg.g = 255;
		spawn_dialog[sdh_size+i].bg.b = 255;
		spawn_dialog[sdh_size+i].bg.a = 255;
		spawn_dialog[sdh_size+i].key = 0;
		spawn_dialog[sdh_size+i].flag = 0;
		spawn_dialog[sdh_size+i].d1 = lista_modelo[id]->icono;
		spawn_dialog[sdh_size+i].d2 = 0;
		spawn_dialog[sdh_size+i].dp = NULL;
		spawn_dialog[sdh_size+i].dp2 = NULL;
		spawn_dialog[sdh_size+i].dp3 = NULL;
		
		i++;
		spawn_dialog[sdh_size+i].df = d_label_proc;
		spawn_dialog[sdh_size+i].x = spawn_dialog[0].x + x;
		spawn_dialog[sdh_size+i].y = spawn_dialog[0].y + y+82;
		spawn_dialog[sdh_size+i].w = 0;
		spawn_dialog[sdh_size+i].h = 0;
		spawn_dialog[sdh_size+i].fg.r = 255;
		spawn_dialog[sdh_size+i].fg.g = 255;
		spawn_dialog[sdh_size+i].fg.b = 255;
		spawn_dialog[sdh_size+i].fg.a = 255;
		spawn_dialog[sdh_size+i].bg.r = 0;
		spawn_dialog[sdh_size+i].bg.g = 0;
		spawn_dialog[sdh_size+i].bg.b = 0;
		spawn_dialog[sdh_size+i].bg.a = 0;
		spawn_dialog[sdh_size+i].key = 0;
		spawn_dialog[sdh_size+i].flag = 0;
		spawn_dialog[sdh_size+i].d1 = 0;
		spawn_dialog[sdh_size+i].d2 = 0;
		spawn_dialog[sdh_size+i].dp = lista_modelo[id]->name;
		spawn_dialog[sdh_size+i].dp2 = fntArial12;
		spawn_dialog[sdh_size+i].dp3 = NULL;
		
		id++;
		x+=120;
		if(x>=spawn_dialog[0].w-80){x=inix; y+=110;}
		
	}
	spawn_dialog[sdh_size+i].df = NULL; // El elemento NULL, debe ser el último de todos y DEBE estar, si no cosas horribles pueden pasar!
	int ret_id=do_dialog(spawn_dialog);
	
	if(ret_id<sdh_size){return -1;}
	
	return (ret_id-sdh_size)/2;
}
#undef sdh_size

void key_handle(SDLKey key, SDLMod mod)
{
	float speed=0.5f;
	float last_x=camera.pos_x;
	float last_y=camera.pos_y;
	float last_z=camera.pos_z;
	
	if (SDL_GetModState() & KMOD_SHIFT){speed=200.0f;}
    
	int i;
	
	switch (key)
    {
    case SDLK_ESCAPE:
		if(camera.ghost_mode==1){camera.ghost_mode=0; break;}
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
		
    case SDLK_a: camera.yaw--;
		break;
    case SDLK_d: camera.yaw++;
		break;
	
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
		
    case SDLK_F10: /* Guarda una foto de la pantalla */
		ilutGLScreenie();
		message_printf(lista_texto[TEXT_LIST_R_DLG + 3]);
		break;
	
    case SDLK_SPACE: /* Menú de spawn */
		if(camera.ghost_mode==1)break;
		i = open_spawn_dialog();
		if(i>=0)
			place_object(i);
		break;
	
    case SDLK_t: /* Funciones de test */
		//node_flow_gas(&node1, &node2, 2.0f, 8.0f, 0.1f);

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
	
	case SDLK_COMMA:
		if(camera.ghost_mode==1)
			rotate_ghost = +15;
		break;
	
	case SDLK_PERIOD:
		if(camera.ghost_mode==1)
			rotate_ghost = -15;
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

/* Obtiene el ID del objeto que está en el trace desde pos en la dirección dir */
int get_traced_object(VECTOR pos, VECTOR dir)
{
	if(dir.z>=0)return -1; // Si miramos hacia arriba o de frente no podemos calcular el punto de corte...
	int obj;
	float sq_l, sq_r, sq_t, sq_b; /* Coordenadas reales del cuadrado del objeto */
	float corte_x, corte_y; /* Coordenadas reales del punto de corte del trace con el plano z = obj.z */
	/* Tipo objeto: base */
	for (obj=0; obj<lista_objetos_base; obj++)
	{
		if (lista_objeto_base[obj]->pos.z < pos.z && vdist_sq(lista_objeto_base[obj]->pos, pos)<MAX_DIST_TRACE_OBJ_SQ) // Nos saltamos los objetos que están demasiado lejos ni por encima nuestro
		{
			sq_l = lista_objeto_base[obj]->pos.x + lista_objeto_base[obj]->sq_l;
			sq_r = lista_objeto_base[obj]->pos.x + lista_objeto_base[obj]->sq_r;
			sq_t = lista_objeto_base[obj]->pos.y + lista_objeto_base[obj]->sq_t;
			sq_b = lista_objeto_base[obj]->pos.y + lista_objeto_base[obj]->sq_b;
			
			
			corte_x = -dir.x/dir.z * pos.z + pos.x; // De la eqn de la recta (Están negados porque salen al revés...)
			corte_y = -dir.y/dir.z * pos.z + pos.y;
			
			if ( (corte_x>=sq_l && corte_x<=sq_r) && (corte_y>=sq_b && corte_y<=sq_t))
			{
				return obj;
			}
		}
	}
	return -1;
}

/* Si se mueve el ratón mientras se está pulsando algun botón del ratón */
void mouse_move_but(int button, int x, int y)
{
	static int dont_use_this_movement;
	float last_x=camera.pos_x;
	float last_y=camera.pos_y;
	float last_z=camera.pos_z;
	
	
	float altura_real = coord_to_real_height(marte,camera.pos_z) - get_real_height(marte, camera.pos_x, camera.pos_y);
	
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
		mouse_static_but(button, x, y);
	}
	
    if(coord_to_real_height(marte,camera.pos_z) - get_real_height(marte, camera.pos_x, camera.pos_y)<0){camera.pos_x=last_x;camera.pos_y=last_y;camera.pos_z=last_z;}
	control();
}

/* Si se pulsa algun botón del ratón y el ratón está quieto */
void mouse_static_but(int button, int x, int y)
{
	
	if (button & SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		if(camera.ghost_mode!=1)
		{
			VECTOR pos = {camera.pos_x,camera.pos_y,camera.pos_z};
			VECTOR dir = v_from_ang(RAD(camera.pitch), RAD(camera.yaw));
			int obj = get_traced_object(pos, dir);
			
			int i;
			for(i=0; i<lista_objetos_base; i++)
			{
				lista_objeto_base[i]->selec=0;
			}
			
			if(obj!=-1) // Si el trace detecta un objeto
			{
				lista_objeto_base[obj]->selec=1;
			}
		}
		else
		{// MODO GHOST
			camera.ghost_mode=2;
		}
	}
}

void clear_cola_eventos(void)
{
	while(SDL_PollEvent(&sdl_event)){}
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
			if (!x && !y){break;}
			mouse_move_but(mstat, x, y);
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (sdl_event.button.button == SDL_BUTTON_WHEELUP)camera.vel_z=+0.2;/* No funciona si no está aquí */
			if (sdl_event.button.button == SDL_BUTTON_WHEELDOWN)camera.vel_z=-0.2;
			mstat = SDL_GetMouseState(&x,&y);
			x-=last_m_x;//scr_width/2;
			y-=last_m_y;//scr_height/2;
			if (!x && !y){mouse_static_but(mstat, x, y);break;}
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
