#include "shared.h"

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
    void *font = GLUT_BITMAP_HELVETICA_12;
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
