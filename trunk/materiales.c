/*
	mars_base - Design, build and admin your own base on Mars
    Copyright (C) 2009  Alfonso Arbona Gimeno (nake90@terra.es). All rights reserved.

	MIT LICENSE
	
	Permission is hereby granted, free of charge, to any
	person obtaining a copy of this software and associated
	documentation files (the "Software"), to deal in the
	Software without restriction, including without limitation
	the rights to use, copy, modify, merge, publish,
	distribute, sublicense, and/or sell copies of the
	Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice
	shall be included in all copies or substantial portions of
	the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
	KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
	WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
	PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
	OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
	OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
	OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
    
    If you use any part of this code you must give me (Alfonso Arbona Gimeno) credit.
    If you plan to use any part of this code on a comercial game please email me at:
	   	   nake90@terra.es
*/

/** \file materiales.c
 * \brief Header que controla los materiales (texturas)
 * \author Alfonso Arbona Gimeno
*/

#include "materiales.h"
#include <IL/ilut.h>

int load_material (t_texture *texture, const char* ruta_)/* return: #0# -> OK; #-1# -> Material not found; #-2# -> Texture not found */
{
	char ruta[256];
	char textura[256];
	str_cpyl(ruta, 256, ruta_);
	str_append(ruta,".nmf");
	FILE* file = fopen(ruta,"r");
	if (!file){return -1;}
	
	fscanf(file,"%f, %f, %f, %f\n",&texture->ambient[0],&texture->ambient[1],&texture->ambient[2],&texture->ambient[3]);
	fscanf(file,"%f, %f, %f, %f\n",&texture->diffuse[0],&texture->diffuse[1],&texture->diffuse[2],&texture->diffuse[3]);
	fscanf(file,"%f, %f, %f, %f\n",&texture->specular[0],&texture->specular[1],&texture->specular[2],&texture->specular[3]);
	fscanf(file,"%f\n",&texture->shininess[0]);
	fscanf(file,"%f\n",&texture->transparency[0]);
	fgets(textura,256,file);
	
	//glGenTextures(1, &texture->texture[0]);
	str_cpyl(texture->name, 80, textura);
	texture->texture[0]=ilutGLLoadImage(textura);
	if(!texture->texture[0]){debug_printf(TL_ERR_MAT_LOAD_TEXTURE,textura);return(-2);}
	
	ilutGLBuildMipmaps();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	return 0;
}
void unload_material (t_texture *texture)
{
	ilBindImage(0);
	ilDeleteImages(1,texture->texture);
	texture->texture[0]=-1;
}
