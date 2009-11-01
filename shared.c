#include "shared.h"

/* - MATH - */
float nabs(float val)
{
	if (val<0){return -val;}
	return val;
}

int nround(float val)
{
	int buff=val;
	if(val-buff>=0.5f){return buff+1;}
	return buff;
}

/* - SCREEN LIST PRINTF - */
void scr_init_reset (int debug_too)
{
	scr_message_index=-1;
	scr_message_debug=debug_too;
	
}

static void scr_init_push (void)
{
	int i;
	if (scr_message_index>0)
	{
		for (i=0; i<scr_message_index-1; i++)
		{
			str_cpy(scr_messages[i], scr_messages[i+1]);
		}
		scr_message_index--;
	}
}

void scr_init_printf (const char *fmt, ...)
{
	char buf[MAX_SCREEN_MESSAGES_LEN];
	va_list args;
	va_start(args, fmt);
	vsprintf (buf, fmt, args);
	va_end(args);
	if (scr_message_index==MAX_SCREEN_MESSAGES-1){scr_init_push();}
	str_cpyl(scr_messages[scr_message_index],MAX_SCREEN_MESSAGES_LEN,buf);
	scr_message_index++;
	/* Printing */
	if (scr_message_debug){debug_printf(scr_messages[scr_message_index-1]);}
	int i;
	int pos=0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (i=scr_message_index-1; i>=0; i--)
	{
		hud_printf (1, pos, scr_messages[i]);
		pos++;
	}
	glFlush();
	glutSwapBuffers();
}

void scr_init_reprintf (const char *fmt, ...)
{
	char buf[MAX_SCREEN_MESSAGES_LEN];
	va_list args;
	va_start(args, fmt);
	vsprintf (buf, fmt, args);
	va_end(args);
	scr_message_index--;
	if (scr_message_index==MAX_SCREEN_MESSAGES-1){scr_init_push();}
	str_cpyl(scr_messages[scr_message_index],MAX_SCREEN_MESSAGES_LEN,buf);
	scr_message_index++;
	/* Printing */
	if (scr_message_debug){debug_printf(scr_messages[scr_message_index-1]);}
	int i;
	int pos=0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (i=scr_message_index-1; i>=0; i--)
	{
		hud_printf (1, pos, scr_messages[i]);
		pos++;
	}
	glFlush();
	glutSwapBuffers();
}



/* - STRINGS - */
int str_size(const char* string)
{
	int i=0;
	while(string[i]!=0){i++;}
	return i;
}
void str_cpy(char* string1,const char* string2)
{
	int i=0;
	while(string2[i]!=0){string1[i]=string2[i];i++;}
	string1[i]=0;
}
void str_cpyl(char* string1, int len, const char* string2)
{
	int i=0;
	while(string2[i]!=0 && i<len){string1[i]=string2[i];i++;}
	string1[i]=0;
}
void str_append(char* string1,const char* string2)
{
	int i=str_size(string1);
	int j=0;
	while(string2[j]!=0){string1[i]=string2[j];i++;j++;}
	string1[i]=0;
}

/* - VECTORS - */
VECTOR p_vect(VECTOR vec1,VECTOR vec2) /* Producto vectorial (cross) */
{
	VECTOR resultado;
	resultado.x = vec1.y*vec2.z -vec1.z*vec2.y;
	resultado.y = vec1.z*vec2.x -vec1.x*vec2.z;
	resultado.z = vec1.x*vec2.y -vec1.y*vec2.x;
	return resultado;
}

float p_escalar(VECTOR vec1,VECTOR vec2) /* Producto escalar (dot) */
{
	return vec1.x*vec2.x + vec1.y*vec2.y + vec1.z*vec2.z;
}
void normalize(VECTOR* vec1)
{
	if (vec1->x||vec1->y||vec1->z)
	{
		float modulo = sqrt(vec1->x*vec1->x + vec1->y*vec1->y + vec1->z*vec1->z);
		vec1->x/=modulo;
		vec1->y/=modulo;
		vec1->z/=modulo;
	}
}
VECTOR vadd(VECTOR vec1, VECTOR vec2)
{
	VECTOR res;
	res.x=vec1.x+vec2.x;
	res.y=vec1.y+vec2.y;
	res.z=vec1.z+vec2.z;
	return res;
}
VECTOR vsub(VECTOR vec1, VECTOR vec2)
{
	VECTOR res;
	res.x=vec1.x-vec2.x;
	res.y=vec1.y-vec2.y;
	res.z=vec1.z-vec2.z;
	return res;
}

/* - DEBUG - */
void debug_reset (void)
{
    FILE* file;
    file=fopen("debug.log","w");
    fprintf(file," --- DEBUG --- \n\n");
    fclose(file);
}

void debug_printf (const char *fmt, ...)
{
    static char buf[256];
    FILE* file;
    file=fopen("debug.log","a");
    va_list args;
    va_start(args, fmt);
    vfprintf (file, fmt, args);
    va_end(args);
    fclose(file);
}

/* - PRINTING - */
void position_printf (float x, float y, float z, const char *fmt, ...)
{
    glDisable(GL_TEXTURE_2D);
	static char buf[256];
    int viewport[4];
    void *font = GLUT_STROKE_ROMAN;
    va_list args;

    va_start(args, fmt);
    (void) vsprintf (buf, fmt, args);
    va_end(args);
    
	glDisable(GL_TEXTURE_2D);
	
    glGetIntegerv(GL_VIEWPORT,viewport);

    //glRotatef(90, 1.0f,0,0);
    glTranslatef(x+1,y+1,z-1);


    //glPushMatrix();
    //glLoadIdentity();

    //glMatrixMode(GL_PROJECTION);
    //glPushMatrix();
    //glLoadIdentity();

        glOrtho(0,viewport[2],0,viewport[3],-1,1);
        
        glColor3d(1.0,1.0,1.0);
        /*glRasterPos2i(
              glutBitmapWidth(font, ' ') * col,
            - glutBitmapHeight(font) * (row+1) + viewport[3]
        );*/
        glutStrokeString (font, (unsigned char *) buf);
        //glColor3d(1.0,1.0,1.0);

    //glPopMatrix();
    //glMatrixMode(GL_MODELVIEW);
    //glPopMatrix();
}

void hud_printf (int col, int row, const char *fmt, ...)
{
    glDisable(GL_TEXTURE_2D);
	static char buf[256];
    int viewport[4];
    //void *font = GLUT_BITMAP_HELVETICA_12;
    void *font = GLUT_BITMAP_9_BY_15;
    va_list args;

    va_start(args, fmt);
    (void) vsprintf (buf, fmt, args);
    va_end(args);
    
	glDisable(GL_TEXTURE_2D);
	
    glGetIntegerv(GL_VIEWPORT,viewport);

    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

        glOrtho(0,viewport[2],0,viewport[3],-1,1);
        
        glColor3d(1.0,1.0,1.0);
        glRasterPos2i(
              glutBitmapWidth(font, ' ') * col,
            - glutBitmapHeight(font) * (row+1) + viewport[3]
        );
        glutBitmapString (font, (unsigned char *) buf);
        glColor3d(1.0,1.0,1.0);

    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void use_texture (t_texture texture)
{
	glEnable(GL_TEXTURE_2D);
	glMaterialfv(GL_FRONT, GL_AMBIENT,   texture.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,   texture.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR,  texture.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, texture.shininess);
	glBindTexture(GL_TEXTURE_2D, texture.texture[0] );
}
