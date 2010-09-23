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

/** \file heightmap.c
 * \brief Carga los heightmaps (mapas)
 * \author Alfonso Arbona Gimeno
*/
#include "heightmap.h"
#include <math.h>
#include <stdio.h>
#include <GL/gl.h>
#define ILUT_USE_OPENGL
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
//#include <pthread.h>
//#include <GL/glu.h>
//#include <SDL/SDL.h>
//#include <SDL/SDL_ttf.h>

t_heightmap marte;

// Variables y funciones privadas para no regenerar las normales varias veces.
unsigned char norm_generated = 0;
VECTOR *norm_data = NULL;

static
void norm_create(int w, int h)
{
	if(norm_generated)
	{
		free(norm_data);
	}
	norm_data = malloc(sizeof(VECTOR)*w*h*2);
	norm_generated = 1;
}

static
void norm_unload(void)
{
	if(norm_generated)
	{
		free(norm_data);
	}
	norm_generated = 0;
}

// Fin variables y funciones privadas de las normales.


static
VECTOR calc_normal(VECTOR vec1,VECTOR vec2)
{
    VECTOR resultado;
    resultado=p_vect(vec1,vec2);
    normalize(&resultado);
    return resultado;
}
/*
static
int sumatorio_4(int a) //!< Calcula SUM(i=0 hasta 'a'){4^i}
{
	int i, res=1;
	for(i=1; i<=a; i++)
	{
		res+=pow(4,i);
	}
	return res;
}*/
/*
static
void quads_generate_parent_list(t_heightmap* obj, unsigned int level, int parent_id)
{
	if(level >= marte.num_quads)return;
	
	obj->quadtree[level].parent = parent_id;
	quads_generate_parent_list(obj, obj->quadtree[level].child[0], level);
	quads_generate_parent_list(obj, obj->quadtree[level].child[1], level);
	quads_generate_parent_list(obj, obj->quadtree[level].child[2], level);
	quads_generate_parent_list(obj, obj->quadtree[level].child[3], level);
}

static
int heightmap_generate_quads(t_heightmap* obj)
{
	float iter;
	unsigned char init = 0;
	int sx = obj->tam_x -1;
	int pow_mult;
	int lado, i; // Los uso después si el mapa no es cuadrado.
	// despl_? define un desplazamiento lineal a la hora de usar los vértices, de
	// forma que si el mapa no es cuadrado el proceso se repita varias veces
	
	ifdebug(DEBUG_INFO){debug_printf("Generando quads\n");}
	
	*/
	/*! El número de quads se calcula así:
	  num_quads = SUM(i de 0 a nºiteraciones){4^i}
	  siendo nºiteraciones = log2(lado_mapa) = ln(lado_mapa)/ln(2)
	  Como el mapa puede no ser cuadrado, se calcula para el lado menor y se
	  multiplica por lado_mayor/lado_menor. Que es entero ya que el mapa es
	  múltiplo de 2. Muahahahaaaa soy DIOS xD, todo esto lo he sacado yo sin
	  internet ni na de eso. Gracias Cálculo Infinitesimal e Integral xD
	*/
	/*

	if(obj->tam_x == obj->tam_y)
	{
		iter = log((double)sx)/0.69314718056; // log(2)

		if(nround(iter*1000.0f)/1000.0f != iter)
		{
			debug_printf("Error, el tamaño del mapa no es multiplo de 2 +1\n > Lado = %i\n > iter = %.4f\n", obj->tam_x, iter);
			return 4;
		}
		obj->max_detail = nround(iter);
		obj->max_detail_rand = obj->max_detail + MAP_DETAIL_RANDOM;

		obj->num_quads = sumatorio_4(obj->max_detail_rand);
		obj->num_parents = 1;
		lado=sx+1;
	}
	else
	{
		debug_printf("Solo mapas cuadrados!\n");
		return 3;
	}

	obj->quadtree=malloc(sizeof(t_map_quad)*obj->num_quads);

	//! Creamos los quadtrees
	int level = 0, pow2=0, id_ant=0, id_ant2=0;
	int incr, x,y;
	pow_mult = pow(2,MAP_DETAIL_RANDOM); // Se utiliza cuando hay random, 'ampliamos' el mapa para que la subdivisión siempre tenga de lado 1
	debug_printf("pow_mult = %i\n", pow_mult);
	obj->pow_mult_scale = pow_mult;
	randomize(666);
	obj->seed = frand();
	//obj->scale /= pow_mult;
	incr=1;
	
	
	ifdebug(DEBUG_DEBUG)
	{
		debug_printf(" > max_details = %i\n > max_detail_rand = %i\n", obj->max_detail, obj->max_detail_rand);
		debug_printf(" > num_quads = %i\n > num_parents = %i\n", obj->num_quads, obj->num_parents);
		debug_printf(" > lado = %i\n > incr_inicial = %i\n", lado, incr);
	}
	
	#define xy2i(x,y) ((x)+(y)*(lado * pow_mult))
	// En juego: x->Izquierda a derecha, y->Detrás a delante; En vértices: x->Iz. a der. y-> Arriba a abajo! (creo...)
	
	// Empiezo con la máxima subdivisión y voy subiendo hasta llegar al quad padre.
	i=0;
	int c1=0, c2=0;
	do
	{
		if(level>0)
		{
			pow2 = pow(2,level-1);
			ifdebug(DEBUG_VERBOSE)debug_printf("Level: %i, map_size: %i, id_ant: %i\n", level, (obj->tam_x -1) / pow2, id_ant);
		}
		for(y=0; y<(obj->tam_y-1)*pow_mult; y+=incr)
		{
		  for(x=0; x<(obj->tam_x-1)*pow_mult; x+=incr)
		  {
		  	if(level >= MAP_DETAIL_RANDOM) // Mod quads con datos
		  	{
					obj->quadtree[i].vertex[0] = xy2i(x,y);		// Arriba izquierda
					obj->quadtree[i].vertex[1] = xy2i(x,y+incr);		// Abajo izquierda
					obj->quadtree[i].vertex[2] = xy2i(x+incr,y+incr);	// Abajo derecha
					obj->quadtree[i].vertex[3] = xy2i(x+incr,y);		// Arriba derecha
					//obj->quadtree[i].modo = 0;
					c1++;
		  	}
		  	else // Modo quads random
		  	{
		  		obj->quadtree[i].rand_quad.seed = frand();
		  		obj->quadtree[i].rand_quad.x = x;
		  		obj->quadtree[i].rand_quad.y = y;
		  		obj->quadtree[i].rand_quad.lado = incr;
					//obj->quadtree[i].modo = 1;
					c2++;
		  	}
		  	
				if(init==0) // Máxima división posible (sin childs)
				{
					obj->quadtree[i].child[0] = obj->num_quads;	// Si child es el último se considera nulo.
					obj->quadtree[i].child[1] = obj->num_quads;
					obj->quadtree[i].child[2] = obj->num_quads;
					obj->quadtree[i].child[3] = obj->num_quads;
				}
				else // Tienen childs
				{
					obj->quadtree[i].child[0] = ((x/(incr/2.0)))  +((y/(incr/2.0)))  *((obj->tam_x -1) / pow2) + id_ant;
					obj->quadtree[i].child[1] = ((x/(incr/2.0)))  +((y/(incr/2.0))+1)*((obj->tam_x -1) / pow2) + id_ant;
					obj->quadtree[i].child[2] = ((x/(incr/2.0))+1)+((y/(incr/2.0))+1)*((obj->tam_x -1) / pow2) + id_ant;
					obj->quadtree[i].child[3] = ((x/(incr/2.0))+1)+((y/(incr/2.0)))  *((obj->tam_x -1) / pow2) + id_ant;
				}
				
				// Vecinos
				obj->quadtree[i].vecino[0] = -1;
				obj->quadtree[i].vecino[1] = -1;
				obj->quadtree[i].vecino[2] = -1;
				obj->quadtree[i].vecino[3] = -1;
				
				if(x>0)
					obj->quadtree[i].vecino[1] = i - 1;
				if(x<(obj->tam_x-1)*pow_mult)
					obj->quadtree[i].vecino[3] = i + 1;
				if(y>0)
					obj->quadtree[i].vecino[0] = i - obj->tam_x/incr;
				if(y<(obj->tam_y-1)*pow_mult)
					obj->quadtree[i].vecino[2] = i + obj->tam_x/incr;
				
				
				obj->quadtree[i].level = obj->max_detail_rand - level;
				
				*/
				/* ESTO NO LO QUIERES DESCOMENTAR!!!! Seguro! Son muuuchos quads!!
				#WARNING Lo advierto!! Son demasiados quads!! No quieres hacerlo!!
					if(incr >= 1)*//* if(init==0 && x==0 && y==0)
						debug_printf("Quad %i (level %i): vertex[0]=[%i](%i,%i), lado=%i, vecinos: %i, %i, %i, %i\n",
								 i,obj->quadtree[i].level,obj->quadtree[i].vertex[0],x,y,incr, obj->quadtree[i].vecino[0],obj->quadtree[i].vecino[1],
								 obj->quadtree[i].vecino[2],obj->quadtree[i].vecino[3]);//*/
								 /*if(x==0 && y==8)
					if(init==0 && x==16 && y==16)
						debug_printf("Quad %i (level %i): vertex[0]=[%i](%i,%i), lado=%i, vecinos: %i, %i, %i, %i\n",
								 i,obj->quadtree[i].level,obj->quadtree[i].vertex[0],x,y,incr, obj->quadtree[i].vecino[0],obj->quadtree[i].vecino[1],
								 obj->quadtree[i].vecino[2],obj->quadtree[i].vecino[3]);//*/
								 /*if(x==0 && y==8)
						debug_printf("Quad %i (level %i): vertex[0]=[%i](%i,%i), lado=%i, vecinos: %i, %i, %i, %i\n",
								 i,obj->quadtree[i].level,obj->quadtree[i].vertex[0],x,y,incr, obj->quadtree[i].vecino[0],obj->quadtree[i].vecino[1],
								 obj->quadtree[i].vecino[2],obj->quadtree[i].vecino[3]);//*/
					/*else*//* if(init==0 && x==0 && y==0)
						debug_printf("Quad %i (level %i): seed=%f, lado=%.4f, childs: %i, %i, %i, %i\n",
								 i,obj->quadtree[i].level,obj->quadtree[i].rand_quad.seed,obj->quadtree[i].rand_quad.lado, obj->quadtree[i].child[0],obj->quadtree[i].child[1],
								 obj->quadtree[i].child[2],obj->quadtree[i].child[3]);
				//*/
				/*
				if(i>=obj->num_quads) // Por si acaso... No debería cumplirse nunca, pero mejor prevenir
				{
					debug_printf("Error, hay más quads de los calculados!\n");
					free(obj->quadtree);
					return 3;
				}
				i++;
		  }
		}

		id_ant = id_ant2;
		id_ant2 = i;
		init=1;
		incr*=2;
		level++;
	}while(incr<=lado);
	ifdebug(DEBUG_DEBUG)debug_printf("Quads calculados: %i\n", i);
	ifdebug(DEBUG_DEBUG)debug_printf(" >> Quads normales %i\n >> Quads random: %i\n", c1, c2);
	#undef xy2i
	
	quads_generate_parent_list(obj, marte.num_quads-1, -1);
	
	
	return 0;
}
*/

int get_traced_coord(VECTOR pos, VECTOR dir, VECTOR *coord)
{
    int contador = 0;
    float despl_x=0.0f, despl_y=0.0f;
    VECTOR v1, v2, v3;
    MATRIX33f A,InvA;
    MATRIX13f B,R;
    
    do
    {
    	get_current_triangle(marte, camera.pos_x + despl_x, camera.pos_y + despl_y, &v1, &v2, &v3);
    	matrix_set_all33f(&A,
											(-dir.x), (v2.x-v1.x), (v3.x-v1.x),
											(-dir.y), (v2.y-v1.y), (v3.y-v1.y),
											(-dir.z), (v2.z-v1.z), (v3.z-v1.z));
			if(matrix_inv33f(&InvA,A)!=0)return -1;
			
			matrix_set_all13f(&B, pos.x-v1.x, pos.y-v1.y, pos.z-v1.z);
			matrix_mult_33fx13f(&R,InvA,B);
			
			// Cutre pero vale por ahora
			despl_x += dir.x * marte.scale*0.25;
			despl_y += dir.y * marte.scale*0.25;
			
			contador++;
    }while(R.a[1]+R.a[2]<=1  &&  contador <= 5);
    
    if(contador >5){return -2;}
		
		coord->x = pos.x + dir.x*R.a[0];
		coord->y = pos.y + dir.y*R.a[0];
		coord->z = pos.z + dir.z*R.a[0];
    
    
    /*
    Triángulos a comprobar:
    Primero el de debajo de la cámara, después el que tiene un lado
    que se corta con la proyección de 'dir', así hasta que el punto
    que corta con el plano está dentro del triángulo que queremos.
    */
    return 0;
}

/*! \fn int load_heightmap(const char* filename, t_heightmap* h_buffer, t_texture texture)
 *  \brief Función principal para la carga de heightmaps
 *	Esta función intenta primero cargar el heightmap precompilado, si no lo encuentra lo compila y lo guarda.
 *	\warning La ruta al archivo no debe contener extensión ya que se usa la función append para añadir la extensión que
 *	se requiera en cada caso.
 *  \param filename Ruta a la carpeta y nombre del archivo SIN EXTENSIÓN
 *  \param h_buffer Puntero al heightmap a crear
 *  \param texture Textura que se usará en el mapa
 *  \return 0 -> Ok
 *	\return 1 -> Imagen de alturas (.pgm) no encontrado
 *	\return 2 -> Archivo de información (.txt) no encontrado
 *	\return 3 -> Fallo en el malloc
 *	\return 4 -> El archivo de alturas no es un PGM
 *
*/
int load_heightmap(const char* filename, t_heightmap* h_buffer, t_texture texture)
{
    char str_buffer[1024];/* Añade la extensión */
    int res;
    int i=0;
    char val[2];
    char line[255];

    /* Cargamos el minimapa */
    str_cpyl(str_buffer,1024-str_len("_minimap.jpg"),filename); // Nos aseguramos que no se pasa de tamaño

    str_append(str_buffer,"_minimap.jpg");
    minimapa=ilutGLLoadImage(str_buffer);
    if(!minimapa)debug_printf(lista_texto[TEXT_LIST_R_WRN + 0],str_buffer);

    /* Intentamos abrir el archivo precompilado */
    str_cpyl(str_buffer,1024-str_len(".nhmap"),filename);
    str_append(str_buffer,".nhmap");
    res=load_compiled_map(str_buffer, h_buffer, texture);

    if(res==0)
    {
        return 0;
    }
    if(res==3)
    {
        return 3;    /* Fallo en el malloc */
    }

    /* Si estamos aquí es porque no se ha podido cargar el precompilado... */

    str_cpy(str_buffer,filename);
    str_append(str_buffer,".pgm");
    FILE* data = fopen(str_buffer,"r");
    if (!data)
    {
        debug_printf(TL_ERR_GEN_OPEN,str_buffer);
        return 1;
    }

    str_cpy(str_buffer,filename);
    str_append(str_buffer,".txt");
    FILE* info = fopen(str_buffer,"r");
    if (!info)
    {
        debug_printf(TL_ERR_GEN_OPEN,str_buffer);
        return 2;
    }



    scr_init_printf (lista_texto[TEXT_LIST_R_SCR + 4]);

    scr_init_printf(lista_texto[TEXT_LIST_R_SCR + 14]);
    /* Cargamos los datos básicos */
    // ToDo: Usar el parser!
    fscanf(info,"North\t= %f\n",&(h_buffer->north));
    fscanf(info,"South\t= %f\n",&(h_buffer->south));
    fscanf(info,"East\t= %f\n",&(h_buffer->east));
    fscanf(info,"West\t= %f\n",&(h_buffer->west));
    fscanf(info,"Scale\t= %f\n",&(h_buffer->scale));
    fscanf(info,"Lines\t= %i\n",&(h_buffer->tam_y));
    fscanf(info,"Samples\t= %i\n",&(h_buffer->tam_x));
    fscanf(info,"zero_h\t= %i\n",&(h_buffer->zero_h));
    fscanf(info,"min_h\t= %i\n",&(h_buffer->min_h));
    fscanf(info,"max_h\t= %i\n",&(h_buffer->max_h));
    fscanf(info,"ini_x\t= %i\n",&(h_buffer->ini_x));
    fscanf(info,"ini_y\t= %i\n",&(h_buffer->ini_y));
    fscanf(info,"ini_z\t= %i\n",&(h_buffer->ini_z));
    fclose(info);
    

    h_buffer->scale*=1000.0f;

    h_buffer->list=0;
#ifdef __OLD_NMAP_MODE__
    h_buffer->data=malloc(sizeof(unsigned char)*h_buffer->tam_y*h_buffer->tam_x);
    if(!h_buffer->data)
    {
        debug_printf(TL_ERR_MAP_MALLOC,h_buffer->tam_y,h_buffer->tam_x);
        return 3;
    }
    h_buffer->shadow=malloc(sizeof(float)*h_buffer->tam_y*h_buffer->tam_x);
    if(!h_buffer->data)
    {
        debug_printf(TL_ERR_MAP_MALLOC,h_buffer->tam_y,h_buffer->tam_x);
        return 3;
    }
    h_buffer->normal=malloc(sizeof(VECTOR)*h_buffer->tam_y*h_buffer->tam_x);
    if(!h_buffer->data)
    {
        debug_printf(TL_ERR_MAP_MALLOC,h_buffer->tam_y,h_buffer->tam_x);
        return 3;
    }

    scr_init_reprintf(lista_texto[TEXT_LIST_R_SCR + 5]);

    fscanf(data,"%c%c\n",&(val[0]),&(val[1]));
    if(val[0]!='P'||val[1]!='2')
    {
        debug_printf(TL_ERR_MAP_PGM_TYPE,val[0],val[1]);
        return 4;
    }

    do
    {
        fgets(line,255,data);
    }
    while(line[0]=='#');/* Leemos una línea. Si hay un '#' implica que era un comentario... */
    //fscanf(data,"%i\n",&i);
    i=0;
    //FILE *deb = fopen("map_debug.txt","w");
    while(fscanf(data,"%i\n",(int *)&(h_buffer->data[i]))!=EOF && i<h_buffer->tam_y*h_buffer->tam_x)
    {
        /*fprintf(deb,"%i\n",h_buffer->data[i]);*/i++;
    }
#else
	h_buffer->vertex = malloc(sizeof(t_map_vertex) * h_buffer->tam_x * h_buffer->tam_y);
	if(!h_buffer->vertex)
	{
		debug_printf(TL_ERR_MAP_MALLOC,h_buffer->tam_y,h_buffer->tam_x);
		return 3;
	}

	scr_init_reprintf(lista_texto[TEXT_LIST_R_SCR + 5]);

    fscanf(data,"%c%c\n",&(val[0]),&(val[1]));
    if(val[0]!='P'||val[1]!='2')
    {
        debug_printf(TL_ERR_MAP_PGM_TYPE,val[0],val[1]);
        return 4;
    }

    do
    {
        fgets(line,255,data);
    }
    while(line[0]=='#');/* Leemos una línea. Si hay un '#' implica que era un comentario... */
    //fscanf(data,"%i\n",&i);
    i=0;
    //FILE *deb = fopen("map_debug.txt","w");
    while(i<h_buffer->tam_y*h_buffer->tam_x && fscanf(data,"%i\n",&res)!=EOF)
    {
        h_buffer->vertex[i].altura = res;
        //fprintf(deb,"%i\n",h_buffer->vertex[i].altura);
        i++;
    }
    //fclose(deb);
#endif
    fclose(data);

    compile_map(h_buffer, texture);
    str_cpy(str_buffer,filename);
    str_append(str_buffer,".nhmap");
    scr_init_printf(lista_texto[TEXT_LIST_R_SCR + 6]);
    save_compiled_map(str_buffer, *h_buffer);
    return 0;
}

void list_compile_map(t_heightmap* obj, t_texture texture)
{
#ifdef __OLD_DISPLAY_MODE__
    int x,y;
    int half_x, half_y;
    half_x = obj->tam_x/2;
    half_y = obj->tam_y/2;

#define MAP_Z_OFFSET 0.20

    float v_scale = (obj->max_h-obj->min_h)/255.0f;
    float v_add = -(obj->vertex[half_x+half_y*obj->tam_x].altura-obj->zero_h)*v_scale + MAP_Z_OFFSET;
    float color=1.0f;
    scr_init_printf (lista_texto[TEXT_LIST_R_SCR + 7]);

    /*
    Necesitamos extensión opengl
    max textures : glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB)
    seleccionar textura: glActiveTextureARB(GL_TEXTUREi_ARB)
          glMultiTexCoord2fARB(GL.GL_TEXTURE0_ARB, 0, 1);
          glMultiTexCoord2fARB(GL.GL_TEXTURE1_ARB, 0+xOffset, 1+yOffset);
    */
    obj->list=glGenLists(3);

    glNewList(obj->list,GL_COMPILE);
    glDisable(GL_LIGHTING);
    use_texture(texture);
    glPushMatrix();

    glTranslatef(-obj->ini_x,-obj->ini_y,-obj->ini_z);
    glRotatef(180, 0, 0, 1.0f);

    for (y=0; y<obj->tam_y-1; y++)
    {
        /* DIBUJO DEL TERRENO TEXTURIZADO */
        glEnable(GL_TEXTURE_2D);
        glBegin(GL_TRIANGLE_STRIP);
        for (x=0; x<obj->tam_x; x++)
        {
            color=obj->vertex[x+y*obj->tam_x].shadow;
            if(color>=0)
            {
                glColor3f(color,color,color);
            }
            else
            {
                glColor3f(1.0,0,1.0);
            }
            /* Dibujamos el primer triángulo */
            // Lo de obj->tam_x-x+1 es porque sale todo como en un espejo en las x's
            glTexCoord2i( (obj->tam_x-x+1 - half_x)*obj->scale, (y - half_y)  *obj->scale);
            glVertex3f  ( (obj->tam_x-x+1 - half_x)*obj->scale, (y - half_y)  *obj->scale, (obj->vertex[x+y*obj->tam_x].altura-obj->zero_h)*v_scale+v_add);

            color=obj->vertex[x+(y+1)*obj->tam_x].shadow;
            if(color>=0)
            {
                glColor3f(color,color,color);
            }
            else
            {
                glColor3f(1.0,0,1.0);
            }
            /* Dibujamos el segundo triángulo */
            glTexCoord2i( (obj->tam_x-x+1 - half_x)*obj->scale, (y+1 - half_y)*obj->scale);
            glVertex3f  ( (obj->tam_x-x+1 - half_x)*obj->scale, (y+1 - half_y)*obj->scale, (obj->vertex[x+(y+1)*obj->tam_x].altura-obj->zero_h)*v_scale+v_add);
        }
        glEnd();
    }
    glPopMatrix();
    glEndList();

    glNewList(obj->list+1,GL_COMPILE);
    use_texture(texture);
    glPushMatrix();
    glTranslatef(-obj->ini_x,-obj->ini_y,-obj->ini_z);
    glRotatef(180, 0, 0, 1.0f);
    for (y=0; y<obj->tam_y-1; y++)
    {
        /* DIBUJO DE TRIANGULOS (wireframe) */
        glDisable(GL_TEXTURE_2D);
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        glBegin(GL_TRIANGLE_STRIP);
        glColor3f(0.0,0,0.0);
        for (x=0; x<obj->tam_x; x++)
        {
            glVertex3f  ( (obj->tam_x-x+1 - half_x)*obj->scale, (y - half_y)  *obj->scale, (obj->vertex[x+y*obj->tam_x].altura-obj->zero_h)*v_scale+v_add);
            glVertex3f  ( (obj->tam_x-x+1 - half_x)*obj->scale, (y+1 - half_y)*obj->scale, (obj->vertex[x+(y+1)*obj->tam_x].altura-obj->zero_h)*v_scale+v_add);
        }
        glEnd();
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }
    glPopMatrix();
    glEndList();

    glNewList(obj->list+2,GL_COMPILE);
    //use_texture(texture);
    glPushMatrix();
    glTranslatef(-obj->ini_x,-obj->ini_y,-obj->ini_z);
    glRotatef(180, 0, 0, 1.0f);

#if 0
    for (y=0; y<obj->tam_y-1; y++)
    {
        /* DIBUJO DE LAS NORMALES y la dirección del sol*/
        glDisable(GL_TEXTURE_2D);
        glBegin(GL_LINES);
        for (x=obj->tam_x-1; x>=0; x--)
        {
            /* Normales */
            glColor3f(1.0,0,1.0);
            glVertex3f  ( (obj->tam_x-x+1                                - half_x)*obj->scale, (y- half_y)  *obj->scale, (obj->vertex[x+y*obj->tam_x].altura-obj->zero_h)*v_scale+v_add);
            glVertex3f  ( (obj->tam_x-x+1- half_x)*obj->scale + obj->normal[x+y*obj->tam_x].x*200, (y- half_y)  *obj->scale+obj->normal[x+y*obj->tam_x].y*200, (obj->vertex[x+y*obj->tam_x].altura-obj->zero_h)*v_scale+v_add+obj->normal[x+y*obj->tam_x].z*200);
            /* Rayos *//*
            glColor3f(0.0,1.0,0.0);
            glVertex3f  ( (obj->tam_x-x+1      - half_x)*obj->scale, (y - half_y)  *obj->scale, (obj->data[x+y*obj->tam_x]-obj->zero_h)*v_scale+v_add);
            glVertex3f  ( (obj->tam_x-x+1- half_x)*obj->scale-ray.x*200, (y- half_y)  *obj->scale-ray.y*200, (obj->data[x+y*obj->tam_x]-obj->zero_h)*v_scale-ray.z*200+v_add);//*/
        }
        glEnd();//*/
    }
#endif
    glPopMatrix();
    glEndList();
    
#else // !__OLD_DISPLAY_MODE__
	
	int x,y;
	float v_scale = (obj->max_h-obj->min_h)/255.0f;
	
	float luz=1.0f;
    scr_init_printf (lista_texto[TEXT_LIST_R_SCR + 7]);
    
    
    obj->list=glGenLists(3); // Son 3 lists, pero solo usaré una...

    glNewList(obj->list,GL_COMPILE);
    glDisable(GL_LIGHTING); // Agiliza los cálculos muy significativamente
    use_texture(texture); //! TODO: Cada triángulo una, y a ser posible con transiciones
    glPushMatrix();

    //glTranslatef(-obj->ini_x,-obj->ini_y,-obj->ini_z); // YA NO SE USA
    //glRotatef(0, 0.0f, 0.0f, 0.0f); // Comprobar si se necesita!!!!
    
    for (y=0; y<obj->tam_y-1; y++) // No usa quads!! Este es modo antiguo, dibuja los vértices y ya está
    {
        /* DIBUJO DEL TERRENO TEXTURIZADO */
        glEnable(GL_TEXTURE_2D);
        glBegin(GL_TRIANGLE_STRIP);
        for (x=0; x<obj->tam_x; x++)
        {
            luz=obj->vertex[x+y*obj->tam_x].shadow;
            if(luz>=0)
            {
                glColor3f(luz,luz,luz);
            }
            else
            {
                glColor3f(1.0,0,1.0);
            }
            /* Dibujamos el primer triángulo */
            // Lo de obj->tam_x-x es porque sale todo como en un espejo en las x's
            glTexCoord2i( (obj->tam_x-x)*obj->scale, (y)  *obj->scale);
            glVertex3f  ( (obj->tam_x-x)*obj->scale, (y)  *obj->scale, (obj->vertex[x+y*obj->tam_x].altura)*v_scale);

            luz=obj->vertex[x+(y+1)*obj->tam_x].shadow;
            if(luz>=0)
            {
                glColor3f(luz,luz,luz);
            }
            else
            {
                glColor3f(1.0,0,1.0);
            }
            /* Dibujamos el segundo triángulo */
            glTexCoord2i( (obj->tam_x-x)*obj->scale, (y+1)*obj->scale);
            glVertex3f  ( (obj->tam_x-x)*obj->scale, (y+1)*obj->scale, (obj->vertex[x+(y+1)*obj->tam_x].altura)*v_scale);
        }
        glEnd();
    }
    glPopMatrix();
    glEndList();
	
	
	
    glNewList(obj->list+1,GL_COMPILE);
    glEndList();
    glNewList(obj->list+2,GL_COMPILE);
    glEndList();
#endif
}



int load_compiled_map(const char* ruta, t_heightmap* obj, t_texture texture)
{
    int magic_ = ((char)'n')+((char)'h'<<8)+((char)'m'<<16);
    scr_init_printf (lista_texto[TEXT_LIST_R_SCR + 8]);
    int magic;
    unsigned char version;
    FILE* file = fopen(ruta,"rb");
    if (!file)
    {
        debug_printf(lista_texto[TEXT_LIST_R_WRN + 1],ruta);
        return -1;
    }


    /* -- Cabecera -- */
    fread(&magic,sizeof(int),1,file);/* Cadena mágica */
    if (magic!=magic_)
    {
        fclose(file);
        debug_printf(TL_ERR_PRECOMP_TYPE);
        return -2;
    }

    fread(&version,sizeof(unsigned char),1,file);/* Versión del compilado */
    if (version!=NHMAP_VERSION)
    {
        fclose(file);
        debug_printf(TL_ERR_PRECOMP_VER);
        return -3;
    }

    fread(&obj->tam_x,sizeof(int),1,file);
    fread(&obj->tam_y,sizeof(int),1,file);
    fread(&obj->scale,sizeof(float),1,file);
    fread(&obj->zero_h,sizeof(int),1,file);
    fread(&obj->min_h,sizeof(int),1,file);
    fread(&obj->max_h,sizeof(int),1,file);
    fread(&obj->north,sizeof(float),1,file);
    fread(&obj->south,sizeof(float),1,file);
    fread(&obj->east,sizeof(float),1,file);
    fread(&obj->west,sizeof(float),1,file);
    fread(&obj->ini_x,sizeof(int),1,file);
    fread(&obj->ini_y,sizeof(int),1,file);
    fread(&obj->ini_z,sizeof(int),1,file);
    /*
    debug_printf(" -- CARGA_MAPA --\n");
    debug_printf("    Tamaño: %i,%i\n",obj->tam_x,obj->tam_y);
    debug_printf("    Escala: %f\n",obj->scale);
    debug_printf("    Alturas: (Z)%i, (m)%i, M(%i)\n",obj->zero_h,obj->min_h,obj->max_h);
    debug_printf("    Coordenadas: (N)%f, (S)%f, E(%f), O(%f)\n",obj->north,obj->south,obj->east,obj->west);
    debug_printf("    Inicial: (%i,%i,%i)\n",obj->ini_x,obj->ini_y,obj->ini_z);//*/


    /* -- Datos -- */

    /* malloc's */
#ifdef __OLD_NMAP_MODE__
    obj->list=0;
    obj->data=malloc(sizeof(unsigned char)*obj->tam_y*obj->tam_x);
    if(!obj->data)
    {
        debug_printf(TL_ERR_MAP_MALLOC,obj->tam_y,obj->tam_x);
        return 3;
    }
    obj->shadow=malloc(sizeof(float)*obj->tam_y*obj->tam_x);
    if(!obj->shadow)
    {
        free(obj->data);
        debug_printf(TL_ERR_MAP_MALLOC,obj->tam_y,obj->tam_x);
        return 3;
    }
    obj->normal=malloc(sizeof(VECTOR)*obj->tam_y*obj->tam_x);
    if(!obj->normal)
    {
        free(obj->data);
        free(obj->shadow);
        debug_printf(TL_ERR_MAP_MALLOC,obj->tam_y,obj->tam_x);
        return 3;
    }

    /* Normales */
    fread(obj->normal, sizeof(VECTOR), obj->tam_x*obj->tam_y, file);
    /* Shadows */
    fread(obj->shadow, sizeof(float), obj->tam_x*obj->tam_y, file);
    /* Alturas */
    fread(obj->data, sizeof(unsigned char), obj->tam_x*obj->tam_y, file);
    fclose(file);

#else
	int i;

	obj->vertex = malloc(sizeof(t_map_vertex) * obj->tam_x * obj->tam_y);
	if(!obj->vertex){debug_printf("Error de malloc\n"); return 3;}
	for(i=0; i<obj->tam_x*obj->tam_y; i++){obj->vertex[i].altura = 0.0;}

	//marte.vertex[(x) + (y)*marte.tam_x].altura

#if (NHMAP_VERSION == 2)
	unsigned char *data;
	float *shadow;
	VECTOR *normal;

	data=malloc(sizeof(unsigned char)*obj->tam_y*obj->tam_x);
    if(!data)
    {
        free(obj->vertex);
        debug_printf(TL_ERR_MAP_MALLOC,obj->tam_y,obj->tam_x);
        return 3;
    }
    shadow=malloc(sizeof(float)*obj->tam_y*obj->tam_x);
    if(!shadow)
    {
        free(obj->vertex);
        free(data);
        debug_printf(TL_ERR_MAP_MALLOC,obj->tam_y,obj->tam_x);
        return 3;
    }
    normal=malloc(sizeof(VECTOR)*obj->tam_y*obj->tam_x);
    if(!normal)
    {
        free(obj->vertex);
        free(data);
        free(shadow);
        debug_printf(TL_ERR_MAP_MALLOC,obj->tam_y,obj->tam_x);
        return 3;
    }

	//! Leemos los datos
    /* Normales */
    fread(normal, sizeof(VECTOR), obj->tam_x*obj->tam_y, file);
    /* Shadows */
    fread(shadow, sizeof(float), obj->tam_x*obj->tam_y, file);
    /* Alturas */
    fread(data, sizeof(unsigned char), obj->tam_x*obj->tam_y, file);

		for(i=0; i<obj->tam_x*obj->tam_y; i++)
		{
			obj->vertex[i].altura=(double)data[i];
			obj->vertex[i].shadow=shadow[i];
			//! Faltan las normales, están mal hechas, hay que añadirlas al formato del nmap.
		}

		free(data);
		free(shadow);
		free(normal);

#endif
    
    
    //if((i=heightmap_generate_quads(obj))!=0)return i;
#endif
    fclose(file); //! AL QUITAR los endif poner esto antes del 'heightmap_generate_quads()'
    
    /* --- */
    list_compile_map(obj, texture);
    scr_init_printf (lista_texto[TEXT_LIST_R_SCR + 13]);
    
    return 0;
}

void save_compiled_map(const char* ruta, t_heightmap obj)
{
    int magic = ((char)'n')+((char)'h'<<8)+((char)'m'<<16);
    unsigned char version = NHMAP_VERSION;
    FILE* file = fopen(ruta,"wb");
    /* -- Cabecera -- */
    fwrite(&magic,sizeof(int),1,file);
    fwrite(&version,sizeof(unsigned char),1,file);
    fwrite(&obj.tam_x,sizeof(int),1,file);
    fwrite(&obj.tam_y,sizeof(int),1,file);
    fwrite(&obj.scale,sizeof(float),1,file);
    fwrite(&obj.zero_h,sizeof(int),1,file);
    fwrite(&obj.min_h,sizeof(int),1,file);
    fwrite(&obj.max_h,sizeof(int),1,file);
    fwrite(&obj.north,sizeof(float),1,file);
    fwrite(&obj.south,sizeof(float),1,file);
    fwrite(&obj.east,sizeof(float),1,file);
    fwrite(&obj.west,sizeof(float),1,file);
    fwrite(&obj.ini_x,sizeof(int),1,file);
    fwrite(&obj.ini_y,sizeof(int),1,file);
    fwrite(&obj.ini_z,sizeof(int),1,file);
    #ifdef __OLD_NMAP_MODE__
    /* -- Datos -- */
    /* Normales */
    fwrite(obj.normal, sizeof(VECTOR), obj.tam_x*obj.tam_y, file);
    /* Shadows */
    fwrite(obj.shadow, sizeof(float), obj.tam_x*obj.tam_y, file);
    /* Alturas */
    fwrite(obj.data, sizeof(unsigned char), obj.tam_x*obj.tam_y, file);
    #else
    int i;
    VECTOR NULL_VECT = {0,0,0};
    /* -- Datos -- */
    /* Normales */
    for(i=0; i<obj.tam_x*obj.tam_y; i++)
    {
    	fwrite(&NULL_VECT, sizeof(VECTOR), 1, file);
    }
    /* Shadows */
    for(i=0; i<obj.tam_x*obj.tam_y; i++)
    {
    	fwrite(&obj.vertex[i].shadow, sizeof(float), 1, file);
    }
    /* Alturas */
    for(i=0; i<obj.tam_x*obj.tam_y; i++)
    {
    	version = (unsigned char)obj.vertex[i].altura;
    	//debug_printf("%f\n",obj.vertex[i].altura);
    	fwrite(&version, sizeof(unsigned char), 1, file);
    }
    #endif
    fclose(file);
}


void destroy_heightmap(t_heightmap* obj)
{
    #ifdef __OLD_NMAP_MODE__
    if (obj)
    {
        if (obj->data)free(obj->data);
        if (obj->shadow)free(obj->shadow);
        if (obj->normal)free(obj->normal);
        /*if (obj->list>0)*/
        glDeleteLists(obj->list,3);
    }
    #else
    if (obj)
    {
        if (obj->vertex)free(obj->vertex);
        //if (obj->quadtree)free(obj->quadtree);
        /*if (obj->list>0)*/
        glDeleteLists(obj->list,3);
    }
    #endif
}

/* --- */

float z_to_real_height(t_heightmap obj, int z)/* Transforma una z (0-255) a su altura real */
{
    //float v_scale = (obj.max_h-obj.min_h)/255.0f;
    //return ((z-obj.zero_h)-(obj.vertex[obj.tam_x/2+obj.tam_y/2*obj.tam_x].altura-obj.zero_h))*v_scale;
    return (z)*((obj.max_h-obj.min_h)/255.0f);
}

/*int compile_percent;
int compile_gl_status;

static void *loading_print_subrutine(void *no_usado)
{
	while(compile_percent>=0)
	{
		if(compile_gl_status!=0)
		{}
		else{SDL_GL_SwapBuffers();}
		SDL_Delay(20);
	}
}*/

void compile_map(t_heightmap* obj, t_texture texture)
{
    /*compile_percent=0;
    compile_gl_status=0;

    pthread_t display_thread;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_create(&display_thread,&attr,loading_print_subrutine,NULL);*/

    debug_printf(lista_texto[TEXT_LIST_R_NFO + 1]);
    scr_init_printf(lista_texto[TEXT_LIST_R_SCR + 9]);
    scr_init_printf(lista_texto[TEXT_LIST_R_SCR + 10]);
    float carga_estado=0;
    float carga_inc=100.0/obj->tam_y;

    int x,y;
    int half_x, half_y;
    half_x = obj->tam_x/2;
    half_y = obj->tam_y/2;
    
    float color=1.0f;
    VECTOR ray = {0.6,0.7,-1.4};

    /* - NOTA - No uso al final las normales en reflejos porque hace muuucho lag! -> ¡Las uso en las sombras! */
    VECTOR vec1, vec2;/* Vectores que forman las aristas de los triángulos para calcular las normales */
    //VECTOR pos1, pos2;/* Posición de los puntos anteriores */

    //compile_gl_status=1;
    /* Primero calculamos las sombras y las normales */
    /* 'x' e 'y' NUNCA = 0 */
    #ifndef __OLD_NMAP_MODE__
			norm_create(obj->tam_x,obj->tam_y);
		#endif
    
    for (y=1; y<obj->tam_y; y++)
    {
        if(y%64)
        {
            /*compile_percent=carga_estado;*/scr_init_reprintf (lista_texto[TEXT_LIST_R_SCR + 11],carga_estado);
        }
        for (x=1; x<obj->tam_x; x++)
        {
            vec1.x=-1;//pos1.x-x;
            vec1.y=0;//pos1.y-y;
            vec1.z = -obj->vertex[(x-1)+(y-0)*obj->tam_x].altura  +  obj->vertex[x+y*obj->tam_x].altura;/*No entiendo porqué tiene que estar invertido...*/

            vec2.x=0;//pos2.x-x;
            vec2.y=-1;//pos2.y-y;
            vec2.z =  obj->vertex[(x-0)+(y-1)*obj->tam_x].altura  -  obj->vertex[x+y*obj->tam_x].altura;

						#ifdef __OLD_NMAP_MODE__
            obj->normal[x+y*obj->tam_x]=calc_normal(vec1,vec2);
            #else
            norm_data[x+y*obj->tam_x]=calc_normal(vec1,vec2); // Primer triángulo
            
            //norm_data[x+y*obj->tam_x + (obj->tam_x*obj->tam_y)]=calc_normal(vec1,vec2); // Segundo triángulo
            #endif
            color=calc_shadow(x, y, ray, obj, AMBIENTE);
        }
        carga_estado+=carga_inc;
    }
    //compile_gl_status=0;
    #ifndef __OLD_NMAP_MODE__
			norm_unload();
		#endif
    
    //if((x=heightmap_generate_quads(obj))!=0){debug_printf("Error al generar los quads: %i\n");}
    
    list_compile_map(obj, texture);
    scr_init_reprintf (lista_texto[TEXT_LIST_R_SCR + 12]);
    debug_printf(lista_texto[TEXT_LIST_R_NFO + 2]);

    //compile_percent=-10;
    //pthread_join(display_thread, NULL);

    //pthread_attr_destroy(&attr);
}

float calc_shadow(int obj_x, int obj_y, VECTOR ray, t_heightmap* obj, float ambiente)
{
    // NOTA: ESTÁN CAMBIADOS LOS NOMBRES, NO compilará en modo OLD_NMAP
    VECTOR cur_pos = {obj_x,obj_y,obj->vertex[obj_x+obj_y*obj->tam_x].altura};
    VECTOR incr = ray;
    normalize(&incr);
    /* Si es vertical no hace falta calcular nada */
    if(incr.x==0 && incr.y==0)
    {
        obj->vertex[obj_x+obj_y*obj->tam_x].shadow=ambiente;
        return ambiente;
    }
    int sombra=0;
    const int max_incrementos = 200;/* Usado para que no tenga que recorrer todo el mapa... así es más rápido. Aumentar si el mapa tiene montañas muy grandes  */
    int incrementos=0;

    cur_pos=vsub(cur_pos,incr);
    /* Mientras estemos en el mapa y no se haya detectado aun una sombra */
    while(sombra==0 && incrementos<max_incrementos && (cur_pos.x<obj->tam_x && cur_pos.x>0  &&  cur_pos.y<obj->tam_y && cur_pos.y>0 && cur_pos.z<256))
    {
        if(obj->vertex[nround(cur_pos.x)+nround(cur_pos.y)*obj->tam_x].altura+0.0f > nround(cur_pos.z))
        {
            sombra=1;
        }
        cur_pos=vsub(cur_pos,incr);
        incrementos++;
    }
    cur_pos=vadd(cur_pos,incr);
    if (sombra==1)
    {
        obj->vertex[obj_x+obj_y*obj->tam_x].shadow=ambiente + /*nabs(p_escalar(obj->normal[obj_x+obj_y*obj->tam_x],incr))/10*/0.05;
        return obj->vertex[obj_x+obj_y*obj->tam_x].shadow;
    }

    float resultado = nabs(p_escalar(norm_data[obj_x+obj_y*obj->tam_x],incr)) + ambiente;
    

    if (resultado>1)
    {
        resultado=1;
    }
    if (resultado<0)
    {
        resultado=0;
    }
    obj->vertex[obj_x+obj_y*obj->tam_x].shadow=resultado;
    return resultado;
}

/*! \fn float get_real_height(t_heightmap obj, float coord_x, float coord_y)
 *  \brief Función que retorna el valor exacto y real de altura en las coordenadas reales (coord_x,coord_y)
 *  \param obj Mapa del terreno
 *  \param coord_x,coord_y Coordenadas reales del punto a medir
 *  \return Altura real del punto
 *  \return 0 si fuera del mapa
 *
*/
float get_real_height(t_heightmap obj, float coord_x, float coord_y)
{
    float v_scale = (obj.max_h-obj.min_h)/255.0f;
    int x, y; /* Coordenadas (x,y) de la matriz de alturas */
    float h1,h2,h3,h4,h; /* Alturas de los vértices y final */
    float a,b; /* Son los incrementos de posición en cada eje */

    /* h1  h2
       0->/|
       |1/ |
       v/_2|
       h3  h4
    */
    //a=obj.tam_x -(obj.tam_x -(obj.tam_x/2) -(coord_x+obj.ini_x)/obj.scale)+1;
    //b=obj.tam_y -((obj.tam_y/2) +(coord_y+obj.ini_y)/obj.scale) -1;
    a = obj.tam_x - coord_x/obj.scale;
    b = coord_y/obj.scale;
    /* Las operaciones no se pueden simplificar por problemas entre (int) y (float), no sale el mismo resultado */
    ////a=obj.tam_x/2 +(coord_x+obj.ini_x)/obj.scale +1;
    ////b=obj.tam_y/2 -(coord_y+obj.ini_y)/obj.scale -1;


    x=(int)a;
    y=(int)b;
    a-=x;
    b-=y; /* Transformamos 'a' y 'b' en los incrementos, van de [0-1) respecto la dist. relativa en el eje 'x' e 'y' respect. */
    if (x<0 || x>obj.tam_x-1 || y<0 || y>obj.tam_y-1)
    {
        return 0.0f;
    }
    h1=obj.vertex[x+y*obj.tam_x].altura*v_scale;//+obj.min_h;
    h2=obj.vertex[(x+1)+y*obj.tam_x].altura*v_scale;//+obj.min_h;
    h3=obj.vertex[x+(y+1)*obj.tam_x].altura*v_scale;//+obj.min_h;
    h4=obj.vertex[(x+1)+(y+1)*obj.tam_x].altura*v_scale;//+obj.min_h;

    if(a+b<1)/* Primer triángulo h1,h2,h3 */
    {
        h=h1+(a*(h2-h1))+(b*(h3-h1));
    }
    else/* Segundo triángulo h4,h3,h2 */
    {
        h=h4+((1-b)*(h2-h4))+((1-a)*(h3-h4));
    }
    return h;
}

/*! \fn int get_current_triangle(t_heightmap obj, float coord_x, float coord_y, VECTOR *v1, VECTOR *v2, VECTOR *v3)
 *  \brief Retorna las coordenadas en el espacio (de openGL) de los vértices del triángulo en el que está situado (x,y) (Coordenadas de cámara)
 *	Nota: v1 es el vértice que forma 90º con v2 y v3, v2 con v1 es paralelo al eje x, y v3 con v1 es paralelo al eje y
 *  \param obj Mapa del terreno
 *  \param coord_x, coord_y Las coordenadas de la posición sobre la cual se quiere obtener el triángulo en el que están
 *  \param v1, v2, v3 Vectores de posición de cada vértice (En coordenadas de openGL) (SALIDA)
 *	\return 0: Ok
 *	\return no cero: Error
*/
int get_current_triangle(t_heightmap obj, float coord_x, float coord_y, VECTOR *v1, VECTOR *v2, VECTOR *v3)
{
    float v_scale = (obj.max_h-obj.min_h)/255.0f;
    int x, y; /* Coordenadas (x,y) de la matriz de alturas */
    float h1,h2,h3,h4;
    float a,b; /* Son los incrementos de posición en cada eje */
    //float v_add = +(obj.vertex[(obj.tam_x/2)+(obj.tam_y/2)*obj.tam_x].altura-obj.zero_h)*v_scale - obj.min_h;

    /* h1  h2
       0->/|
       |1/ |
       v/_2|
       h3  h4
    */

    //a=obj.tam_x -(obj.tam_x -(obj.tam_x/2) -(coord_x+obj.ini_x)/obj.scale)+1;
    //b=obj.tam_y -((obj.tam_y/2) +(coord_y+obj.ini_y)/obj.scale) -1;
    a = obj.tam_x - coord_x/obj.scale;
    b = coord_y/obj.scale;
    
    
    x=(int)a;
    y=(int)b;
    a-=x;
    b-=y; /* Transformamos 'a' y 'b' en los incrementos, van de [0-1) respecto la dist. relativa en el eje 'x' e 'y' respect. */
    if (x<0 || x>obj.tam_x-1 || y<0 || y>obj.tam_y-1)
    {
        return -1;
    }
    h1=(obj.vertex[x+y*obj.tam_x].altura)*v_scale;					/*(0,0)*/
    h2=(obj.vertex[(x+1)+y*obj.tam_x].altura)*v_scale;			/*(1,0)*/
    h3=(obj.vertex[x+(y+1)*obj.tam_x].altura)*v_scale;			/*(0,1)*/
    h4=(obj.vertex[(x+1)+(y+1)*obj.tam_x].altura)*v_scale;	/*(1,1)*/

    if(a+b<1)/* Primer triángulo h1=v1, h2=v2, h3=v3 */
    {
        v1->x= (obj.tam_x - x  )*obj.scale;
        v1->y= (y+0)*obj.scale;
        v1->z=h1;

        v2->x= (obj.tam_x - x-1)*obj.scale;
        v2->y= (y+0)*obj.scale;
        v2->z=h2;

        v3->x= (obj.tam_x - x  )*obj.scale;
        v3->y= (y+1)*obj.scale;
        v3->z=h3;
    }
    else/* Segundo triángulo h4=v1, h3=v2, h2=v3 */
    {
        v1->x= (obj.tam_x - x-1)*obj.scale;
        v1->y= (y+1)*obj.scale;
        v1->z=h4;

        v2->x= (obj.tam_x - x  )*obj.scale;
        v2->y= (y+1)*obj.scale;
        v2->z=h3;

        v3->x= (obj.tam_x - x-1)*obj.scale;
        v3->y= (y+0)*obj.scale;
        v3->z=h2;
    }
    return 0;
}

#if(0)
void quad_dump_data(const char *filename)
{
	int i;
	int tam_x = marte.tam_x * marte.pow_mult_scale;
	FILE *file = fopen(filename, "w");
	fprintf(file, "Quad DUMP:\n\n");
	
	
	fprintf(file, "\nQuad info:\n");
	fprintf(file, " > max_detail: %i\n", marte.max_detail);
	fprintf(file, " > max_detail_rand: %i\n", marte.max_detail_rand);
	fprintf(file, " > pow_mult_scale: %i\n", marte.pow_mult_scale);
	fprintf(file, " > num_quads: %i\n", marte.num_quads);
	fprintf(file, " > num_parents: %i\n", marte.num_parents);
	
	fprintf(file, "\nQuadtree data:\n");
	for(i=0; i<marte.num_quads; i++)
	{
		fprintf(file, " > [%i].level: %i\n", i, marte.quadtree[i].level);
		/*if(marte.quadtree[i].modo == 0)
		{*/
			fprintf(file, " > [%i].modo: NORMAL\n", i);
			fprintf(file, " > [%i].vertex[0]: %i (%i,%i)\n", i, marte.quadtree[i].vertex[0],  marte.quadtree[i].vertex[0] % tam_x, marte.quadtree[i].vertex[0] / tam_x);
			fprintf(file, " > [%i].vertex[1]: %i (%i,%i)\n", i, marte.quadtree[i].vertex[1],  marte.quadtree[i].vertex[1] % tam_x, marte.quadtree[i].vertex[1] / tam_x);
			fprintf(file, " > [%i].vertex[2]: %i (%i,%i)\n", i, marte.quadtree[i].vertex[2],  marte.quadtree[i].vertex[2] % tam_x, marte.quadtree[i].vertex[2] / tam_x);
			fprintf(file, " > [%i].vertex[3]: %i (%i,%i)\n", i, marte.quadtree[i].vertex[3],  marte.quadtree[i].vertex[3] % tam_x, marte.quadtree[i].vertex[3] / tam_x);
		/*}
		else
		{
			fprintf(file, " > [%i].modo: RANDOM\n", i);
			fprintf(file, " > [%i].seed: %f\n", i, marte.quadtree[i].rand_quad.seed);
			fprintf(file, " > [%i].x,y: (%i,%i)\n", i, marte.quadtree[i].rand_quad.x, marte.quadtree[i].rand_quad.y);
			fprintf(file, " > [%i].lado: %i\n", i, marte.quadtree[i].rand_quad.lado);
		}*/
		// Faltan normales
		fprintf(file, " > [%i].childs: %i, %i, %i, %i\n", i, marte.quadtree[i].child[0], marte.quadtree[i].child[1], marte.quadtree[i].child[2], marte.quadtree[i].child[3]);
	}
	
	fclose(file);
}
#endif
