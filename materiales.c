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

/** \file materiales.c
 * \brief Header que controla los materiales (texturas)
 * \author Alfonso Arbona Gimeno
*/

#include "materiales.h"

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
	if(!texture->texture[0]){return(-2);}
	
	ilutGLBuildMipmaps();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	return 0;
}
int unload_material (t_texture *texture)
{
	//ilDeleteImages(1,
	return 0;
}
