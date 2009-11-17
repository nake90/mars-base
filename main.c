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

#include "mars_base_private.h"

#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>

/* Librerías añadidas a las carpetas 'include' y 'lib' del proyecto */
#include "GL/openglut.h"
#include "IL/ilut.h"

//#include "3dsloader.h"
#include "shared.h"
#include "overlay.h"
#include "heightmap.h"
#include "materiales.h"

/* TODO (#9#): traces y esas cosas */
/* TODO (#7#): Arreglar presión y añadir temperatura */
/* TODO (#7#): Sol */
/* TODO (#7#): Cargar más objetos y poder elegirlos!! */
/* TODO (#7#): Objetos, formato y tal */



/* Número de casillas visibles en modo presión - ## OBSOLETO ##, cuando arregle la presión lo quitaré */
#define PRESION_VISIBLE 30

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
int p_raton_last_pres[2]={0,0};/* Ultima posicion del ratón al pulsar una tecla */



/* VARIABLES DE CONFIGURACIÓN */
//int overlay_posible; Asumo que si se puede... no creo que alguien juegue a un juego 3D y su pantalla no pueda añadir overlays.


//t_3ds_model test;

GLfloat fogColor[4]= {0.81f, 0.64f, 0.61f, 1.0f};
GLfloat fogRange[2]= {25000.0f, 50000.0f};//{120.0f, 500.0f};
/*					AMBIENT						DIFFUSE						SPECULAR		SHININESS TEXTURE */
t_texture sand;//={{0.2f, 0.2f, 0.2f, 1.0f},{0.92f, 0.72f, 0.21f, 1.0f},{0.05f, 0.05f, 0.05f, 1.0f},{1.0},{0}};
/*					AMBIENT					DIFFUSE					SPECULAR				POSITION			HORA   TEXTURES*/
t_sun sun={{0.5f, 0.5f, 0.5f, 1.0f},{1.0f, 1.0f, 1.0f, 1.0f},{1.0f, 1.0f, 1.0f, 1.0f},{20000.0f, 20000.0f, 20000.0f},{12.0f},{0,0}};
/*							AMBIENT						DIFFUSE						SPECULAR		SHININESS TEXTURE */
t_texture sun_texture={{1.0f, 1.0f, 1.0f, 1.0f},{1.0f, 1.0f, 1.0f, 1.0f},{0.0f, 0.0f, 0.0f, 1.0f},{1.0},{0}};

t_heightmap marte;



/* GLUT callback Handlers */

static void resize(int width, int height)
{
	const float ar = (float) width / (float) height;
	/* NORMAL */
	glutUseLayer(GLUT_NORMAL);
	glViewport(0, 0, width, height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,fogRange[1]+100.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	/* OVERLAY */
	glutUseLayer(GLUT_OVERLAY);
	glViewport(0, 0, width, height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,fogRange[1]+100.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glutUseLayer(GLUT_NORMAL);
}

#if(0)
static void display_3ds(t_3ds_model object)
{
	int l_index;
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, object.id_texture); // We set the active texture 

    glRotatef(object.yaw, 0,0,1.0f);
    glRotatef(object.pitch, 1.0f,0,0);
    //glRotatef(object.roll, sin(RAD(object.yaw)),cos(RAD(object.yaw)),-sin(RAD(object.pitch))); ARREGLAR!!!
    
    
    glTranslatef(object.x,object.y,object.z);

    glBegin(GL_TRIANGLES); // glBegin and glEnd delimit the vertices that define a primitive (in our case triangles)
	for (l_index=0;l_index<object.polygons_qty;l_index++)
    {
        //----------------- FIRST VERTEX -----------------
        // Texture coordinates of the first vertex
        glTexCoord2f( object.mapcoord[ object.polygon[l_index].a ].u ,
                      object.mapcoord[ object.polygon[l_index].a ].v );
        // Coordinates of the first vertex
        glVertex3f( object.vertex[ object.polygon[l_index].a ].x *object.size,
                    object.vertex[ object.polygon[l_index].a ].y *object.size,
                    object.vertex[ object.polygon[l_index].a ].z *object.size); //Vertex definition

        //----------------- SECOND VERTEX -----------------
        // Texture coordinates of the second vertex
        glTexCoord2f( object.mapcoord[ object.polygon[l_index].b ].u ,
                      object.mapcoord[ object.polygon[l_index].b ].v );
        // Coordinates of the second vertex
        glVertex3f( object.vertex[ object.polygon[l_index].b ].x *object.size,
                    object.vertex[ object.polygon[l_index].b ].y *object.size,
                    object.vertex[ object.polygon[l_index].b ].z *object.size);
        
        //----------------- THIRD VERTEX -----------------
        // Texture coordinates of the third vertex
        glTexCoord2f( object.mapcoord[ object.polygon[l_index].c ].u ,
                      object.mapcoord[ object.polygon[l_index].c ].v );
        // Coordinates of the Third vertex
        glVertex3f( object.vertex[ object.polygon[l_index].c ].x *object.size,
                    object.vertex[ object.polygon[l_index].c ].y *object.size,
                    object.vertex[ object.polygon[l_index].c ].z *object.size);
    }
    glEnd();
}
#endif
void display(void)
{
    int i;
    int j;
    VECTOR v;
    
    float r,g,b;
    float r2,b2;
    float r3,b3;
    float r4,b4;
	//const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    //const double a = t*90.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glEnable(GL_LIGHTING);
    glEnable(GL_FOG);
    
    glRotatef(-camera.pitch, 1.0f,0,0);
    glRotatef(-camera.yaw, 0,0,1.0f);
    glRotatef(-camera.roll, sin(RAD(-camera.yaw)),cos(RAD(-camera.yaw)),-sin(RAD(camera.pitch)));
    
    glTranslatef(-camera.pos_x,-camera.pos_y,-camera.pos_z);
	
    glLightfv(GL_LIGHT0, GL_POSITION, sun.position);
	
	//use_texture(sand);
	
	glCallList(marte.list);
	
	/* - Display Casillas - FIXME!!!!! (casillas solo de la zona actual, de 1x1m^2) */
   	/*if (show_grid)
	{
		glDisable(GL_TEXTURE_2D);
		glLineWidth(1.5f);
		for (i=-TERR_SIZE; i<=TERR_SIZE; i++)
		{
			if(i!=0){glColor3d(0.7f,0.7f,1.0f);}else{glColor3d(0.0f,0.0f,1.0f);}
			glBegin(GL_LINES);
				glVertex3f( i * 1.0f, TERR_SIZE * 1.0f, 0.0f);
				glVertex3f( i * 1.0f,-TERR_SIZE * 1.0f, 0.0f);
				glVertex3f( TERR_SIZE * 1.0f, i * 1.0f, 0.0f);
				glVertex3f(-TERR_SIZE * 1.0f, i * 1.0f, 0.0f);
			glEnd();
		}
	}*/
	glColor3d(1.0,1.0,1.0);
	
	if (show_grid) /* Ahora muestra la cuadrícula */
	{
		glCallList(marte.list+1);
	}
	if (show_presion) /* Ahora muestra las normales */
	{
		glCallList(marte.list+2);
	}
	
	/* - Display Presión - BORRADO */
	/*if (show_presion && 0)
	{
		glDisable(GL_TEXTURE_2D);
		presion_m=0;
		presion_m_c=0;
		for (j=((int)camera.pos_y)-PRESION_VISIBLE; j<((int)camera.pos_y)+PRESION_VISIBLE; j++)
		for (i=((int)camera.pos_x)-PRESION_VISIBLE; i<((int)camera.pos_x)+PRESION_VISIBLE; i++)
		{
			g=0.1f;
			b =1.0-(get_presion(i+1,j+1)-PRESION_MIN)/(PRESION_MAX-PRESION_MIN);
			r =    (get_presion(i+1,j+1)-PRESION_MIN)/(PRESION_MAX-PRESION_MIN);
			b2=1.0-(get_presion(i  ,j+1)-PRESION_MIN)/(PRESION_MAX-PRESION_MIN);
			r2=    (get_presion(i  ,j+1)-PRESION_MIN)/(PRESION_MAX-PRESION_MIN);
			b3=1.0-(get_presion(i  ,j  )-PRESION_MIN)/(PRESION_MAX-PRESION_MIN);
			r3=    (get_presion(i  ,j  )-PRESION_MIN)/(PRESION_MAX-PRESION_MIN);
			b4=1.0-(get_presion(i+1,j  )-PRESION_MIN)/(PRESION_MAX-PRESION_MIN);
			r4=    (get_presion(i+1,j  )-PRESION_MIN)/(PRESION_MAX-PRESION_MIN);
			presion_m+=get_presion(i,j);
			presion_m_c++;
			glBegin(GL_QUADS);
				glNormal3f( 0.0f, 0.0f, +1.0f);
				
					glColor4d(r,g,b,0.4f);
				glVertex3f( i*1.0f +1.0f,  j*1.0f +1.0f, 0.001f);
					glColor4d(r2,g,b2,0.4f);
				glVertex3f( i*1.0f -0.0f,  j*1.0f +1.0f, 0.001f);
					glColor4d(r3,g,b3,0.4f);
				glVertex3f( i*1.0f -0.0f,  j*1.0f -0.0f, 0.001f);
					glColor4d(r4,g,b4,0.4f);
				glVertex3f( i*1.0f +1.0f,  j*1.0f -0.0f, 0.001f);
			glEnd();
		}
		v=trace_camera(camera.pos_x,camera.pos_y,camera.pos_z);
		glColor3d(0.0f,0.0f,0.0f);
		i=((int)v.x);
		j=((int)v.y);
		glBegin(GL_LINE_LOOP);
				glVertex3f( i*1.0f +1.0f,  j*1.0f +1.0f, 0.001f);
				glVertex3f( i*1.0f -0.0f,  j*1.0f +1.0f, 0.001f);
				glVertex3f( i*1.0f -0.0f,  j*1.0f -0.0f, 0.001f);
				glVertex3f( i*1.0f +1.0f,  j*1.0f -0.0f, 0.001f);
		glEnd();
		
		position_printf ((int)v.x,(int)v.y,1.5, "%i,%i\n%.2f",(int)v.x,(int)v.y,get_presion((int)v.x,(int)v.y));
		glColor3d(0.0,0.0,0.0);
		glPointSize(5.0);
		
		for (i=0;i<CICLONES;i++)
		{
			glBegin(GL_POINTS);
				glVertex3f( ciclon[i].pos_x,  ciclon[i].pos_y, 0.001f);
			glEnd();
		}
	}*/
	
	
	glColor3d(1.0,1.0,1.0);
	
	//display_3ds(test);
	
	
	//glDisable(GL_LIGHTING);
	glDisable(GL_FOG);
	
	/*
    glRotatef(camera.roll, sin(RAD(camera.yaw)),cos(RAD(camera.yaw)),-sin(RAD(camera.pitch)));
    glRotatef(camera.yaw, 0,0,1.0f);
    glRotatef(camera.pitch, 1.0f,0,0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	//glBindTexture(GL_TEXTURE_2D, sun.texture[0]);
	use_texture(sun_texture);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(sun.position[0]-SUN_SIZE, sun.position[1]-SUN_SIZE, sun.position[2]+0.0f);// Bottom Left
		glTexCoord2f(1.0f, 0.0f); glVertex3f(sun.position[0]+SUN_SIZE, sun.position[1]-SUN_SIZE, sun.position[2]+0.0f);// Bottom Right
		glTexCoord2f(1.0f, 1.0f); glVertex3f(sun.position[0]+SUN_SIZE, sun.position[1]+SUN_SIZE, sun.position[2]+0.0f);// Top Right
		glTexCoord2f(0.0f, 1.0f); glVertex3f(sun.position[0]-SUN_SIZE, sun.position[1]+SUN_SIZE, sun.position[2]+0.0f);// Top Left
	glEnd();
	glBlendFunc(GL_ONE,GL_ZERO);
	glDisable(GL_BLEND);*/
	
    glDisable(GL_LIGHTING);

    hud_printf (0, 0, "TEST de casillas isométricas");
    hud_printf (0, 1, "PITCH, YAW, ROLL = (%.2f, %.2f, %.2f)",camera.pitch, camera.yaw, camera.roll);
    hud_printf (0, 2, "POS = (%.2f, %.2f, %.2f)",camera.pos_x, camera.pos_y, camera.pos_z);
    hud_printf (0, 3, "Flechas para moverse, shift para ir muy rápido");
    hud_printf (0, 4, "WASD+QE -> Girar cámara");
    hud_printf (0, 5, "IJKL -> Girar objeto");
    hud_printf (0, 6, "C/V -> Ver/ocultar la cuadrícula");
    hud_printf (0, 7, "B/N -> Ver/ocultar las normales");
	
	/* Como los overlays aun no están implementados en openGLUT, mostramos el HUD sobre la pantalla directamente */
	draw_HUD();
	glutSwapBuffers();
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
		
		/*
	case 'i': test.pitch++;
		break;
    case 'k': test.pitch--;
		break;
		
    case 'o': test.roll++;
		break;
    case 'u': test.roll--;
		break;
		
    case 'j': test.yaw++;
		break;
    case 'l': test.yaw--;
		break;
		*/
	
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
		/*
    case 'z':
		vec=trace_camera(camera.pos_x,camera.pos_y,camera.pos_z);
		presion=get_presion((int)vec.x, (int)vec.y)-0.5;
		set_presion(presion,(int)vec.x, (int)vec.y);
		break;
    case 'x':
		vec=trace_camera(camera.pos_x,camera.pos_y,camera.pos_z);
		presion=get_presion((int)vec.x, (int)vec.y)+0.5;
		set_presion(presion,(int)vec.x, (int)vec.y);
		break;
*/
    default:
        break;
    }
	
    glutPostRedisplay();
}

static void
special_keys(int key, int x, int y)
{
    float speed=0.5f;
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

    glutPostRedisplay();
}

static void
idle(void)
{
    /*update++;
    if (update>9999)//ATM_UPDATE) NO USAR!!!!
    {
		update=0;
		add_noise_presion(1.8f);
		update_ciclon();
	    flow_presion (10.0f);
	}*/
	glutPostRedisplay();
	/*glutPostOverlayRedisplay(); /* NO IMPLEMENTADO AUN EN openGLUT!!!!! */
}

static /* Si se aprieta o se suelta una tecla del ratón */
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
}

static /* Si se mueve el ratón mientras se está pulsando algun botón del ratón */
void mouse_move_but(int x, int y)
{
	float cx, cy;
	int w=glutGet(GLUT_WINDOW_WIDTH);
	int h=glutGet(GLUT_WINDOW_HEIGHT);
	int val;
	float altura_real=camera.pos_z;
	
	if (b_raton[B_DER_RATON] && !b_raton[B_IZQ_RATON])/* Desplazamiento por la pantalla */
	{
		/* cx, cy -> Incrementos de posición del ratón */
		cx=(p_raton_last_pres[0]-x);
		cy=(p_raton_last_pres[1]-y);
		camera.pos_x+=cx*cos(RAD(camera.yaw))*altura_real/1000.0;
		camera.pos_y+=cx*sin(RAD(camera.yaw))*altura_real/1000.0;
		camera.pos_x-=cy*sin(RAD(-camera.yaw))*altura_real/1000.0;
		camera.pos_y-=cy*cos(RAD(-camera.yaw))*altura_real/1000.0;
	}
	
	if (b_raton[B_IZQ_RATON] && b_raton[B_DER_RATON])/* Zoom */
	{
		/* cy -> Incremento de posición del ratón */
		cy=(p_raton_last_pres[1]-y);
		camera.pos_z+=cy*altura_real/1000.0;
	}
	
	if (b_raton[B_CEN_RATON])/* Girar pantalla */
	{
		/* cx, cy -> Incrementos de posición del ratón */
		cx=(p_raton_last_pres[0]-x);
		cy=(p_raton_last_pres[1]-y);
		camera.pitch+=cy/10.0f;
		camera.yaw+=cx/10.0f;
	}
	p_raton_last_pres[0]=x;
	p_raton_last_pres[1]=y;
}

static /* Si se mueve el ratón mientras NO se pulsa algun botón del ratón */
void mouse_move_pas(int x, int y)
{
	
}

static
void salir (void)
{
	destroy_heightmap(&marte);
}


void GLinit(void)
{
	/* Inicio del GLUT*/
	glutInitWindowSize(800,600);
	glutInitWindowPosition(40,40);
	
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE );
	glutCreateWindow("Mars Base v" VER_STRING);
	//glutEstablishOverlay(); /* NO IMPLEMENTADO AUN EN openGLUT!!!!! */
	//glutHideOverlay(); /* NO IMPLEMENTADO AUN EN openGLUT!!!!! */
	glutFullScreen();
	
	/* Funciones por mensaje */
	atexit(salir);
	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	/*glutOverlayDisplayFunc(draw_HUD); /* NO IMPLEMENTADO AUN EN openGLUT!!!!! */
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
	glFogf(GL_FOG_DENSITY, 0.35f);
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
    /* - INICIACIÓN VARIABLES - */
    int i;
    debug_reset();
    debug_printf("Mars_Base v." VER_STRING " - by nake\n\n");
	tam_mapa_x = TERR_SIZE*2;
	tam_mapa_y = TERR_SIZE*2;
    show_grid=0;
    show_presion=0;
    //randomize_presion(PRESION_MEDIA,50);
    
    
    /* - INICIACIÓN PROGRAMA - */
    glutInit(&argc, argv);
	GLinit();
	scr_init_reset(0);

	/* - POSINICIALIZACIÓN - */
	
	i=load_material(&sand, "materials\\sand_default");
	if(i){debug_printf("Error al cargar la textura base!, RETURN:%i\n",i); return(-1);}
	/*sand.texture[0]=ilutGLLoadImage("mars_sand_rocks_2.tga");
	if(!sand.texture[0]){exit(0);}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	ilutGLBuildMipmaps();*/
	
	sun_texture.texture[0]=ilutGLLoadImage("materials\\sun.tga");
	if(!sun_texture.texture[0]){exit(0);}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	ilutGLBuildMipmaps();
	
	
	scr_init_printf ("Iniciando recursos...");
	camera.pitch = 25;
	camera.yaw = 0;
	camera.pos_z=10;
	camera.pos_y=-4.5;
	
	/* Cargamos el sol */
	//sun.texture[0]=ilutGLLoadImage("materials\\sun.tga");
	//sun.texture[1]=ilutGLLoadImage("materials\\sun_mask.tga");
	//if(!sun.texture[0] /*|| !sun.texture[1]*/){return 1;}
	
	scr_init_printf ("Cargando modelos...");
	
	/*if(Load3DS(&test,"models\\test.3ds")!=1){exit(1);}
	test.id_texture=ilutGLLoadImage("materials\\genericmetal05.jpg");
	test.size=1.0f;*/
	
	scr_init_printf ("Cargando terreno...");
	load_heightmap("heightmaps\\marineris",&marte,sand);
	
	init_ciclon();
	
	scr_init_printf ("Iniciado");
	
	glClearColor(fogColor[0],fogColor[1],fogColor[2],1.0f);
    glutMainLoop();

    return 0;
}
