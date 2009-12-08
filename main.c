/*! \mainpage Mars Base
 *
 * \section intro_sec Introducci�n
 *
 * Mars-Base (nombre no definitivo) es un juego que estamos desarrollando que consistir� en dise�ar, construir y mantener una colonia en Marte. Siendo en todo momento lo m�s fieles a la realidad en los aspectos qu�micos, f�sicos y topogr�ficos.
 * Para crear la base se contar� con m�dulos prefabricados (pasillos, puertas, habitaciones, biodomes,...) que se podr�n ensamblar entre s� permitiendo bases de estructura dise�ada por el jugador. A nivel m�s de interiores, se deber� dise�ar el sistema de circuitos el�ctricos (sensores, actuadores y suministro el�ctrico) y de fluidos (aire, agua,...), aunque supongo que se podr�n usar elementos prefabricados.
 * El juego est� siendo desarrollado usando openGLUT y devIL. Por ahora tan solo puede mostrar el mapa tridimensional marciano (Valles Marineris) y cargar objetos simples, pero est� siendo desarrollado con intensidad. 
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
 * Este archivo se encarga de la inicializaci�n del juego y contiene el main central.
 * \author Alfonso Arbona Gimeno
*/
#include "mars_base_private.h"

#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>

/* Librer�as a�adidas a las carpetas 'include' y 'lib' del proyecto */
#include "GL/openglut.h"
#include "IL/ilut.h"

#include "shared.h"
#include "objetos.h"
#include "overlay.h"
#include "heightmap.h"
#include "materiales.h"
#include "atmosferico.h"

static short update=0;

int show_grid;
int show_presion;

enum botones_raton
{
	B_IZQ_RATON,
	B_DER_RATON,
	B_CEN_RATON
};
short int b_raton[3]={0,0,0};
int p_raton_last_pres[2]={0,0};/* Ultima posicion del rat�n al pulsar una tecla */



t_model test_data;
t_model test2_data;
t_obj_base test;
t_obj_base test2;

/* Niebla y atm�sfera */
GLfloat fogColor[4]= {0.81f, 0.64f, 0.61f, 1.0f}; /*!< Color de la niebla */
GLfloat fogRange[2]= {25000.0f, 50000.0f}; /*!< Distancia m�nima de la niebla y distancia m�xima visible en metros, pregunta interesante... distancia m�xima 50Km?? */

/* TEXTURAS B�SICAS */
t_texture sand;
/*					AMBIENT					DIFFUSE					SPECULAR				POSITION			HORA   TEXTURES*/
t_sun sun={{0.5f, 0.5f, 0.5f, 1.0f},{1.0f, 1.0f, 1.0f, 1.0f},{1.0f, 1.0f, 1.0f, 1.0f},{10000.0f, 20000.0f, 30000.0f},{12.0f},{0,0}};
/*							AMBIENT						DIFFUSE						SPECULAR		SHININESS TEXTURE */
t_texture sun_texture={{1.0f, 1.0f, 1.0f, 1.0f},{1.0f, 1.0f, 1.0f, 1.0f},{0.0f, 0.0f, 0.0f, 1.0f},{1.0},{0}};

/* GLUT callback Handlers */

static void resize(int width, int height)
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
	/* Dibujos est�ticos (Sky) */
	
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
	
	if (show_grid) /* Ahora muestra la cuadr�cula */
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
	hud_printf (1, 8, "Altura hasta el suelo: %f",coord_to_real_height(marte,camera.pos_z) - get_real_height(marte, camera.pos_x, camera.pos_y));
	
	glutSwapBuffers();
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

static void
key(unsigned char key, int x, int y)
{
	VECTOR vec;
	float presion;
	switch (key)
    {
    case 27 :
    case 'Q': exit(0);
		 break;
    
    case 'w': camera.pitch++;
		break;
    case 's': camera.pitch--;
		break;
		
    case 'e': camera.roll++;
		break;
    case 'q': camera.roll--;
		break;
		
    case 'a': camera.yaw++;
		break;
    case 'd': camera.yaw--;
		break;
		
	case 'i': test.rot.x++;
		break;
    case 'k': test.rot.x--;
		break;
		
    case 'o': test.rot.z++;
		break;
    case 'u': test.rot.z--;
		break;
		
    case 'j': test.rot.y++;
		break;
    case 'l': test.rot.y--;
		break;
	
    case 'c':
		show_grid=1;
		break;
    case 'v':
		show_grid=0;
		break;
		
    case 'b':
		show_presion=1;
		break;
    case 'n':
		show_presion=0;
		break;
		
    case 't': /* Funciones de test */
		
		break;
		

    default:
        break;
    }
	control();
    glutPostRedisplay();
}

static void
special_keys(int key, int x, int y)
{
    float speed=0.5f;
    float last_x=camera.pos_x;
    float last_y=camera.pos_y;
    float last_z=camera.pos_z;
    if (glutGetModifiers() & GLUT_ACTIVE_SHIFT){speed=200.0f;}
	
	switch (key)
    {
    case GLUT_KEY_UP: 
		camera.pos_x+=speed*sin(RAD(-camera.yaw));
		camera.pos_y+=speed*cos(RAD(-camera.yaw));
		break;
    case GLUT_KEY_DOWN:
		camera.pos_x-=speed*sin(RAD(-camera.yaw));
		camera.pos_y-=speed*cos(RAD(-camera.yaw));
		break;
		
    case GLUT_KEY_RIGHT:
		camera.pos_x+=speed*cos(RAD(camera.yaw));
		camera.pos_y+=speed*sin(RAD(camera.yaw));
		break;
    case GLUT_KEY_LEFT:
		camera.pos_x-=speed*cos(RAD(camera.yaw));
		camera.pos_y-=speed*sin(RAD(camera.yaw));
		break;
		
    case GLUT_KEY_PAGE_UP: camera.pos_z+=speed;
		break;
    case GLUT_KEY_PAGE_DOWN: camera.pos_z-=speed;
		break;

    default:
        break;
    }
	
	/* Evitamos el estar bajo la tierra */
	if(coord_to_real_height(marte,camera.pos_z) - get_real_height(marte, camera.pos_x, camera.pos_y)<0){camera.pos_x=last_x;camera.pos_y=last_y;camera.pos_z=last_z;}
	control();
	
    glutPostRedisplay();
}

static void
idle(void)
{
	glutPostRedisplay();
}

static /* Si se aprieta o se suelta una tecla del rat�n */
void mouse_but(int button, int state,int x, int y)
{
	p_raton_last_pres[0]=x;
	p_raton_last_pres[1]=y;
	if (button==GLUT_LEFT_BUTTON)
	{
		b_raton[B_IZQ_RATON]=(state-GLUT_DOWN)?0:1;
	}
	if (button==GLUT_RIGHT_BUTTON)
	{
		b_raton[B_DER_RATON]=(state-GLUT_DOWN)?0:1;
	}
	if (button==GLUT_MIDDLE_BUTTON)
	{
		b_raton[B_CEN_RATON]=(state-GLUT_DOWN)?0:1;
	}
	control();
}

static /* Si se mueve el rat�n mientras se est� pulsando algun bot�n del rat�n */
void mouse_move_but(int x, int y)
{
	float cx, cy;
	int w=glutGet(GLUT_WINDOW_WIDTH);
	int h=glutGet(GLUT_WINDOW_HEIGHT);
	int val;
	float altura_real = coord_to_real_height(marte,camera.pos_z) - get_real_height(marte, camera.pos_x, camera.pos_y);
	//debug_printf("Altura real: %f; z2real: %f; get_real: %f \n",altura_real,coord_to_real_height(marte,camera.pos_z),get_real_height(marte, camera.pos_x, camera.pos_y));
	
	if (b_raton[B_DER_RATON] && !b_raton[B_IZQ_RATON])/* Desplazamiento por la pantalla */
	{
		/* cx, cy -> Incrementos de posici�n del rat�n */
		cx=(p_raton_last_pres[0]-x);
		cy=(p_raton_last_pres[1]-y);
		camera.pos_x+=cx*cos(RAD(camera.yaw))*altura_real/1000.0;
		camera.pos_y+=cx*sin(RAD(camera.yaw))*altura_real/1000.0;
		camera.pos_x-=cy*sin(RAD(-camera.yaw))*altura_real/1000.0;
		camera.pos_y-=cy*cos(RAD(-camera.yaw))*altura_real/1000.0;
	}
	
	if (b_raton[B_IZQ_RATON] && b_raton[B_DER_RATON])/* Zoom */
	{
		/* cy -> Incremento de posici�n del rat�n */
		cy=(p_raton_last_pres[1]-y);
		camera.pos_z+=cy*altura_real/1000.0;
	}
	
	if (b_raton[B_CEN_RATON])/* Girar pantalla */
	{
		/* cx, cy -> Incrementos de posici�n del rat�n */
		cx=(p_raton_last_pres[0]-x);
		cy=(p_raton_last_pres[1]-y);
		camera.pitch+=cy/10.0f;
		camera.yaw+=cx/10.0f;
	}
	p_raton_last_pres[0]=x;
	p_raton_last_pres[1]=y;
	
	control();
}

static /* Si se mueve el rat�n mientras NO se pulsa algun bot�n del rat�n */
void mouse_move_pas(int x, int y)
{
	
}

static
void salir (void)
{
	destroy_heightmap(&marte);
	model_unload(test.modelo);
	unload_material(&sand);
	unload_material(&sun_texture);
}

static
void GLinit(void)
{
	/* Inicio del GLUT*/
	glutInitWindowSize(800,600);
	glutInitWindowPosition(40,40);
	
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE );
	glutCreateWindow("Mars Base v" VER_STRING);
	glutFullScreen();
	
	/* Funciones por mensaje */
	atexit(salir);
	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(key);
	glutSpecialFunc(special_keys);
	glutIdleFunc(idle);
	glutMouseFunc(mouse_but);
	glutMotionFunc(mouse_move_but);
	glutPassiveMotionFunc(mouse_move_pas);
	
	/* DevIL init */
	ilInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	
	/* OpenGL - Specific */
	glShadeModel(GL_SMOOTH);
	glClearColor(0.02f,0.1f,0.02f,1.0f);
	//glClearColor(fogColor[0],fogColor[1],fogColor[2],1.0f);
	
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








int main(int argc, char *argv[])
{
    /* - INICIACI�N VARIABLES - */
    str_cpy(app_path,argv[0]);
    str_ruta_back(app_path);
    int i;
    debug_reset();
    debug_printf("Mars_Base v." VER_STRING " (%s) - by nake\n\n",app_path);
	tam_mapa_x = TERR_SIZE*2;
	tam_mapa_y = TERR_SIZE*2;
    show_grid=0;
    show_presion=0;
    
    
    /* - INICIACI�N PROGRAMA - */
    glutInit(&argc, argv);
	GLinit();
	scr_init_reset(0);

	/* - POSINICIALIZACI�N (Carga elementos) - */
	scr_init_printf ("Cargando materiales...");
	
	i=load_material(&sand, "materials\\sand_default");
	if(i){debug_printf("Error al cargar la textura base!, RETURN:%i\n",i); return(-1);}
	
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
	
	
	scr_init_printf ("Iniciado");
	
	glClearColor(fogColor[0],fogColor[1],fogColor[2],1.0f);
	
	
	/* - MAIN LOOP - */
    glutMainLoop();

    return 0;
}
