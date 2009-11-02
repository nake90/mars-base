#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>

/* Librerías añadidas a las carpetas 'include' y 'lib' del proyecto */
#include "GL/openglut.h"
#include "IL/ilut.h"

#include "3dsloader.h"
#include "shared.h"
#include "overlay.h"
#include "heightmap.h"

/* TODO (#9#): Ratón, traces y esas cosas */
/* TODO (#7#): Arreglar presión y añadir temperatura */
/* TODO (#1#): Sombras */


/* Casillas_textura/metros_que_representa (0.5 -> media textura equivale a un metro) */
#define TEX_SIZE 0.25

/* Número de casillas visibles en modo presión */
#define PRESION_VISIBLE 30

#include "atmosferico.h"

static short update=0;

int show_grid;
int show_presion;

t_3ds_model test;

GLfloat fogColor[4]= {0.81f, 0.64f, 0.61f, 1.0f};
GLfloat fogRange[2]= {25000.0f, 50000.0f};//{120.0f, 500.0f};
/*					AMBIENT						DIFFUSE						SPECULAR		SHININESS TEXTURE */
t_texture sand={{0.2f, 0.2f, 0.2f, 1.0f},{0.92f, 0.72f, 0.21f, 1.0f},{0.05f, 0.05f, 0.05f, 1.0f},{1.0},{0}};
/*					AMBIENT					DIFFUSE					SPECULAR				POSITION			HORA   TEXTURES*/
t_sun sun={{0.5f, 0.5f, 0.5f, 1.0f},{1.0f, 1.0f, 1.0f, 1.0f},{1.0f, 1.0f, 1.0f, 1.0f},{2.0f, 5.0f, 5.0f, 0.0f},{12.0f},{0,0}};

t_heightmap marte;

/* - FUNCIONES BASE - */
/* Trace */
VECTOR trace_camera(float x, float y, float z)
{
	VECTOR vec={0,0,0};
	float d;
	if (camera.pitch>=80 || camera.pitch<=-80){return vec;}
	d=tan(RAD(camera.pitch))*z;
	vec.y=d*cos(RAD(-camera.yaw))+y;
	vec.x=d*sin(RAD(-camera.yaw))+x;
	return vec;
}

/* GLUT callback Handlers */

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,fogRange[1]+100.0f);
    /*if( ar > .5 )
        glFrustum( -ar, ar, -1.0, 1.0, 2.0, 100.0 );
    else
        glFrustum( -1.0, 1.0, -1/ar, 1/ar, 2.0, 100.0 );*/
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

static void display_3ds(t_3ds_model object)
{
	int l_index;
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, object.id_texture); // We set the active texture 

    glRotatef(object.yaw, 0,0,1.0f);
    glRotatef(object.pitch, 1.0f,0,0);
    //glRotatef(object.roll, sin(RAD(object.yaw)),cos(RAD(object.yaw)),-sin(RAD(object.pitch)));
    
    
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
    
    glRotatef(-camera.pitch, 1.0f,0,0);
    glRotatef(-camera.yaw, 0,0,1.0f);
    glRotatef(-camera.roll, sin(RAD(-camera.yaw)),cos(RAD(-camera.yaw)),-sin(RAD(camera.pitch)));
    
    glTranslatef(-camera.pos_x,-camera.pos_y,-camera.pos_z);
	
    glLightfv(GL_LIGHT0, GL_POSITION, sun.position);
	
	use_texture(sand);
	
	glCallList(marte.list);
	
	/*
	//glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glBindTexture(GL_TEXTURE_2D, sun.texture[0]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.1f, -1.1f, 1.0f);// Bottom Left
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.1f, -1.1f, 1.0f);// Bottom Right
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.1f, 1.1f, 1.0f);// Top Right
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.1f, 1.1f, 1.0f);// Top Left
	glEnd();
	glBlendFunc(GL_ONE,GL_ZERO);
	glDisable(GL_BLEND);
	
	/* - Display Casillas - FIXME!!!!!*/
   	if (show_grid)
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
	}
	glColor3d(1.0,1.0,1.0);
	
	/* - Display Presión - BORRADO */
	if (show_presion && 0)
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
	}
	glColor3d(1.0,1.0,1.0);
	
	display_3ds(test);
    glDisable(GL_LIGHTING);

    hud_printf (0, 0, "TEST de casillas isométricas");
    hud_printf (0, 1, "PITCH, YAW, ROLL = (%.2f, %.2f, %.2f)",camera.pitch, camera.yaw, camera.roll);
    hud_printf (0, 2, "POS = (%.2f, %.2f, %.2f)",camera.pos_x, camera.pos_y, camera.pos_z);
    hud_printf (0, 3, "Flechas para moverse, shift para ir muy rápido");
    hud_printf (0, 4, "WASD+QE -> Girar cámara");

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
    update++;
    if (update>9999)//ATM_UPDATE) NO USAR!!!!
    {
		update=0;
		add_noise_presion(1.8f);
		update_ciclon();
	    flow_presion (10.0f);
	}
	glutPostRedisplay();
}



void salir (void)
{
	destroy_heightmap(&marte);
}



/* Program entry point */

int main(int argc, char *argv[])
{
    /* - INICIACIÓN VARIABLES - */
    debug_reset();
	tam_mapa_x = TERR_SIZE*2;
	tam_mapa_y = TERR_SIZE*2;
    show_grid=0;
    show_presion=0;
    //randomize_presion(PRESION_MEDIA,50);
    
    /* - INICIACIÓN PROGRAMA - */
    atexit(salir);
	glutInitWindowSize(800,600);
    glutInitWindowPosition(40,40);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE );

    glutCreateWindow("TEST isométria - nake");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(special_keys);
    glutIdleFunc(idle);
    
    glutFullScreen();
    glutSetMenu(1); 
    glutAddMenuEntry("TEST",1);
    
	ilInit();
    ilutInit();
    ilutRenderer(ILUT_OPENGL);
    
    /* - CREATE MATERIAL! - */
	sand.texture[0]=ilutGLLoadImage("mars_sand_rocks_2.tga");
	if(!sand.texture[0]){exit(0);}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//iluBuildMipmaps();
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 16, 16,GL_RGB, GL_UNSIGNED_BYTE, sand.texture[0]);
	ilutGLBuildMipmaps();
	
    glShadeModel(GL_SMOOTH);
	glClearColor(fogColor[0],fogColor[1],fogColor[2],1.0f);
	
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

	/* - POSINICIALIZACIÓN - */
	scr_init_reset(0);
	scr_init_printf ("Iniciando recursos...");
	camera.pitch = 25;
	camera.yaw = 0;
	camera.pos_z=10;
	camera.pos_y=-4.5;
	
	/* Cargamos el sol */
	sun.texture[0]=ilutGLLoadImage("materials\\sun.tga");
	//sun.texture[1]=ilutGLLoadImage("materials\\sun_mask.tga");
	if(!sun.texture[0] /*|| !sun.texture[1]*/){return 1;}
	
	scr_init_printf ("Cargando modelos...");
	
	if(Load3DS(&test,"models\\test.3ds")!=1){exit(1);}
	test.id_texture=ilutGLLoadImage("materials\\genericmetal05.jpg");
	test.size=1.0f;
	
	scr_init_printf ("Cargando terreno...");
	load_heightmap("heightmaps\\marineris",&marte,sand);
	
	init_ciclon();
	
	scr_init_printf ("Iniciado");
	
    glutMainLoop();

    return 0;
}
