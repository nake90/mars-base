#include "heightmap.h"

static VECTOR calc_normal(VECTOR vec1,VECTOR vec2)
{
	VECTOR resultado;
	resultado=p_vect(vec1,vec2);
	normalize(&resultado);
	return resultado;
}

int load_heightmap(const char* filename, t_heightmap* h_buffer)
{
	char str_buffer[255];/* A�ade la extensi�n */
	scr_init_printf(" > Cargando datos b�sicos");
	h_buffer->list=0;
	
	str_cpy(str_buffer,filename);
	str_append(str_buffer,".pgm");
	FILE* data = fopen(str_buffer,"r"); if (!data){debug_printf("### Fallo al abrir \"%s\"",str_buffer);return 1;}
	
	str_cpy(str_buffer,filename);
	str_append(str_buffer,".txt");
	FILE* info = fopen(str_buffer,"r"); if (!info){debug_printf("### Fallo al abrir \"%s\"",str_buffer);return 2;}
	
	/* Cargamos los datos b�sicos */
	// Hay que currarse un cargador decente no tan secuencial...
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
	
	h_buffer->data=malloc(sizeof(int)*h_buffer->tam_y*h_buffer->tam_x);
	if(!h_buffer->data){debug_printf("### Fallo en el malloc de los datos de altura (%i*%i)",h_buffer->tam_y,h_buffer->tam_x);return 3;}
	h_buffer->shadow=malloc(sizeof(float)*h_buffer->tam_y*h_buffer->tam_x);
	if(!h_buffer->data){debug_printf("### Fallo en el malloc de los datos de sombras (%i*%i)",h_buffer->tam_y,h_buffer->tam_x);return 3;}
	h_buffer->normal=malloc(sizeof(VECTOR)*h_buffer->tam_y*h_buffer->tam_x);
	if(!h_buffer->data){debug_printf("### Fallo en el malloc de los datos de normales (%i*%i)",h_buffer->tam_y,h_buffer->tam_x);return 3;}
	
	scr_init_reprintf(" > Cargando alturas");
	
	int i=0;
	char val[2];
	int vali[2];
	char line[255];
	fscanf(data,"%c%c\n",&(val[0]),&(val[1])); if(val[0]!='P'||val[1]!='2'){debug_printf("### Fallo, no es un PGM (%c%c)",val[0],val[1]);return 4;}
	
	do{fgets(line,255,data);}
	while(line[0]=='#');/* Leemos una l�nea. Si hay un '#' implica que era un comentario... */
	fscanf(data,"%i\n",&i);
	i=0;
	while(fscanf(data,"%i\n",&(h_buffer->data[i]))!=EOF && i<h_buffer->tam_y*h_buffer->tam_x){i++;}
	fclose(data);
	return 0;
}


void destroy_heightmap(t_heightmap* obj)
{
	free(obj->data);
	free(obj->shadow);
	free(obj->normal);
	glDeleteLists(obj->list,1);
}

/* --- */

/*static float _get_real_height (int z, t_heightmap obj)/* Transforma una z (0-255) a su altura real *//*
{
	float v_scale = (obj.max_h-obj.min_h)/255.0f;
	return ((z-obj.zero_h)-(obj.data[obj.tam_x/2+obj.tam_y/2*obj.tam_x]-obj.zero_h))*v_scale;
}
float get_real_height (int x, int y, t_heightmap obj)/* Obtiene el valor de la altura real de una casilla no centrada (0->tama�o_mapa) */
/*{
	if (x<0 || x>obj.tam_x || y<0 || y>obj.tam_y){return 0;}
	return _get_real_height(obj.data[x+y*obj.tam_x],obj);
}*/

float calc_shadow (int obj_x, int obj_y, VECTOR ray, t_heightmap* obj, float ambiente)
{
	VECTOR cur_pos = {obj_x,obj_y,obj->data[obj_x+obj_y*obj->tam_x]};
	VECTOR incr = ray; normalize(&incr);
	/* Si es vertical no hace falta calcular nada */
	if(incr.x==0 && incr.y==0){obj->shadow[obj_x+obj_y*obj->tam_x]=ambiente; return ambiente;}
	int sombra=0;
	const int max_incrementos = 200;/* Usado para que no tenga que recorrer todo el mapa... as� es m�s r�pido. Aumentar si el mapa tiene monta�as muy grandes  */
	int incrementos=0;
	
	cur_pos=vsub(cur_pos,incr);
	/* Mientras estemos en el mapa y no se haya detectado aun una sombra */
	while(sombra==0 && incrementos<max_incrementos && (cur_pos.x<obj->tam_x && cur_pos.x>0  &&  cur_pos.y<obj->tam_y && cur_pos.y>0 && cur_pos.z<256))
	{
		if(obj->data[nround(cur_pos.x)+nround(cur_pos.y)*obj->tam_x]+0.0f > nround(cur_pos.z)){sombra=1;}
		cur_pos=vsub(cur_pos,incr);
		incrementos++;
	}
	cur_pos=vadd(cur_pos,incr);
	if (1 && sombra==1){debug_printf(" -- SOMBRA:(%i,%i,%i) Corte:(%i,%i,%i) Altura:%.2f Loops: %i RAY:(%.2f,%.2f,%.2f)\n",
					obj_x , obj_y , obj->data[obj_x+obj_y*obj->tam_x],
					nround(cur_pos.x) , nround(cur_pos.y) , obj->data[nround(cur_pos.x)+nround(cur_pos.y)*obj->tam_x],
					cur_pos.z,
					incrementos,
					ray.x,ray.y,ray.z);}
	if (sombra==1){obj->shadow[obj_x+obj_y*obj->tam_x]=ambiente + nabs(p_escalar(obj->normal[obj_x+obj_y*obj->tam_x],incr))/10; return obj->shadow[obj_x+obj_y*obj->tam_x];}
	
	float resultado = nabs(p_escalar(obj->normal[obj_x+obj_y*obj->tam_x],incr)) + ambiente;
	
	if (resultado>1){resultado=1;}
	if (resultado<0){resultado=0;}
	obj->shadow[obj_x+obj_y*obj->tam_x]=resultado;
	return resultado;
}

int load_compiled_map(const char* ruta, t_heightmap* obj, t_texture texture)
{
	scr_init_reprintf (" > Cargando datos");
	char magic[10];
	FILE* file = fopen(ruta,"rb");
	/* -- Cabecera -- */
	fscanf(file,"%s",&magic);/* Cadena m�gica */
	if (strcmp("nhmap",magic)!=0){fclose(file); debug_printf("ERROR: No es un nhmap (Mapa precompilado)");exit(-1);return -1;}
	fread(&obj->tam_x,sizeof(int),1,file);
	fread(&obj->tam_y,sizeof(int),1,file);
	//fprintf(file,"%i%i",obj.tam_x,obj.tam_y);
	fread(&obj->scale,sizeof(float),1,file);
	//fprintf(file,"%f",obj.scale);
	fread(&obj->zero_h,sizeof(int),1,file);
	fread(&obj->min_h,sizeof(int),1,file);
	fread(&obj->max_h,sizeof(int),1,file);
	//fprintf(file,"%i%i%i",obj.zero_h,obj.min_h,obj.max_h);
	fread(&obj->north,sizeof(float),1,file);
	fread(&obj->south,sizeof(float),1,file);
	fread(&obj->east,sizeof(float),1,file);
	fread(&obj->west,sizeof(float),1,file);
	//fprintf(file,"%f%f%f%f",obj.north,obj.south,obj.east,obj.west);
	fread(&obj->ini_x,sizeof(int),1,file);
	fread(&obj->ini_y,sizeof(int),1,file);
	fread(&obj->ini_z,sizeof(int),1,file);
	//fprintf(file,"%i%i%i",obj.ini_x,obj.ini_y,obj.ini_z);
	/* -- Datos -- */
	/* Normales */
	fread(obj->normal, sizeof(VECTOR), obj->tam_x*obj->tam_y, file);
	/* Shadows */
	fread(obj->shadow, sizeof(float), obj->tam_x*obj->tam_y, file);
	/* Alturas */
	fread(obj->data, sizeof(int), obj->tam_x*obj->tam_y, file);
	fclose(file);
	
	
	
	/* --- */
	int x,y;
	int half_x, half_y;
	half_x = obj->tam_x/2;
	half_y = obj->tam_y/2;
	
	float v_scale = (obj->max_h-obj->min_h)/255.0f;
	float v_add = -(obj->data[half_x+half_y*obj->tam_x]-obj->zero_h)*v_scale;
	float color=1.0f;
	
	scr_init_reprintf (" > Predibujando terreno");
	/* DIBUJAMOS */
	obj->list=glGenLists(1);
	glDisable(GL_LIGHTING);
	glNewList(obj->list,GL_COMPILE);
		use_texture(texture);
		glPushMatrix();
		glTranslatef(-obj->ini_x,-obj->ini_y,-obj->ini_z);
		glRotatef(180, 0, 0, 1.0f);
		
		for (y=0;y<obj->tam_y-1; y++)
		{
			/* DIBUJO DEL TERRENO TEXTURIZADO */
			glEnable(GL_TEXTURE_2D);
			glBegin(GL_TRIANGLE_STRIP);
			for (x=0;x<obj->tam_x; x++)
			{
				color=obj->shadow[x+y*obj->tam_x];
				if(color>=0){glColor3f(color,color,color);}else{glColor3f(1.0,0,1.0);}
				if(y==0 && x==0){glColor3f(0.0,1,0.0);}
				/* Dibujamos el primer tri�ngulo */
				// Lo de obj->tam_x-x+1 es porque sale todo como en un espejo en las x's
				glTexCoord2i( (obj->tam_x-x+1 - half_x)*obj->scale, (y - half_y)  *obj->scale);
				glVertex3f  ( (obj->tam_x-x+1 - half_x)*obj->scale, (y - half_y)  *obj->scale, (obj->data[x+y*obj->tam_x]-obj->zero_h)*v_scale+v_add);
				
				color=obj->shadow[x+(y+1)*obj->tam_x];
				if(color>=0){glColor3f(color,color,color);}else{glColor3f(1.0,0,1.0);}
				if(y==0 && x==0){glColor3f(0.0,1,0.0);}
				/* Dibujamos el segundo tri�ngulo */
				glTexCoord2i( (obj->tam_x-x+1 - half_x)*obj->scale, (y+1 - half_y)*obj->scale);
				glVertex3f  ( (obj->tam_x-x+1 - half_x)*obj->scale, (y+1 - half_y)*obj->scale, (obj->data[x+(y+1)*obj->tam_x]-obj->zero_h)*v_scale+v_add);
			}
			glEnd();
		}
		glPopMatrix();
	glEndList();
	scr_init_reprintf (" > Finalizado");
	
	return 0;
}

void save_compiled_map(const char* ruta, t_heightmap obj)
{
	FILE* file = fopen(ruta,"wb");
	/* -- Cabecera -- */
	fprintf(file,"nhmap\0");/* Cadena m�gica */
	fwrite(&obj.tam_x,sizeof(int),1,file);
	fwrite(&obj.tam_y,sizeof(int),1,file);
	//fprintf(file,"%i%i",obj.tam_x,obj.tam_y);
	fwrite(&obj.scale,sizeof(float),1,file);
	//fprintf(file,"%f",obj.scale);
	fwrite(&obj.zero_h,sizeof(int),1,file);
	fwrite(&obj.min_h,sizeof(int),1,file);
	fwrite(&obj.max_h,sizeof(int),1,file);
	//fprintf(file,"%i%i%i",obj.zero_h,obj.min_h,obj.max_h);
	fwrite(&obj.north,sizeof(float),1,file);
	fwrite(&obj.south,sizeof(float),1,file);
	fwrite(&obj.east,sizeof(float),1,file);
	fwrite(&obj.west,sizeof(float),1,file);
	//fprintf(file,"%f%f%f%f",obj.north,obj.south,obj.east,obj.west);
	fwrite(&obj.ini_x,sizeof(int),1,file);
	fwrite(&obj.ini_y,sizeof(int),1,file);
	fwrite(&obj.ini_z,sizeof(int),1,file);
	//fprintf(file,"%i%i%i",obj.ini_x,obj.ini_y,obj.ini_z);
	/* -- Datos -- */
	/* Normales */
	fwrite(obj.normal, sizeof(VECTOR), obj.tam_x*obj.tam_y, file);
	/* Shadows */
	fwrite(obj.shadow, sizeof(float), obj.tam_x*obj.tam_y, file);
	/* Alturas */
	fwrite(obj.data, sizeof(int), obj.tam_x*obj.tam_y, file);
	fclose(file);
}

void create_map(t_heightmap* obj, t_texture texture)
{
	float carga_estado=0;
	float carga_inc=100.0/obj->tam_y;
	int valores_guardados=0;
	
	int x,y;
	int half_x, half_y;
	half_x = obj->tam_x/2;
	half_y = obj->tam_y/2;
	
	float v_scale = (obj->max_h-obj->min_h)/255.0f;
	float v_add = -(obj->data[half_x+half_y*obj->tam_x]-obj->zero_h)*v_scale;
	float color=1.0f;
	VECTOR ray = {0.6,0.7,-1.4};
	
	/* - NOTA - No uso al final las normales en reflejos porque hace muuucho lag! -> �Las uso en las sombras! */
	VECTOR vec1, vec2, vec3;/* Vectores que forman las aristas de los tri�ngulos para calcular las normales */
	VECTOR pos1, pos2;/* Posici�n de los puntos anteriores */
	
	/* Primero calculamos las sombras y las normales */
	/* 'x' e 'y' NUNCA = 0 */
	//for (y=1;y<obj->tam_y; y++)
	for (y=300;y<500; y++)
	{
		if(y%64)scr_init_reprintf (" > Calculando sombras y normales (%3.0f%%)",carga_estado);
		//for (x=1;x<obj->tam_x; x++)
		for (x=400;x<600; x++)
		{
				vec1.x=-1;//pos1.x-x;
				vec1.y=0;//pos1.y-y;
				vec1.z = -obj->data[(x-1)+(y-0)*obj->tam_x]  +  obj->data[x+y*obj->tam_x];/*No entiendo porqu� tiene que estar invertido...*/
				
				vec2.x=0;//pos2.x-x;
				vec2.y=-1;//pos2.y-y;
				vec2.z =  obj->data[(x-0)+(y-1)*obj->tam_x]  -  obj->data[x+y*obj->tam_x];
				
				obj->normal[x+y*obj->tam_x]=calc_normal(vec1,vec2);
				color=calc_shadow(x, y, ray, obj, AMBIENTE);
				
				if(0 && valores_guardados<20 && x==600 && y>500){valores_guardados++;
				debug_printf("(%i) ---\n  Vector1:(%.2f,%.2f,%.2f)\n  Vector2:(%.2f,%.2f,%.2f)\n  NORMAL:(%.2f,%.2f,%.2f)\n  COORD(%i,%i)\n"
				,valores_guardados,vec1.x,vec1.y,vec1.z,vec2.x,vec2.y,vec2.z,obj->normal[x+y*obj->tam_x].x,obj->normal[x+y*obj->tam_x].y,obj->normal[x+y*obj->tam_x].z,x,y);
				obj->shadow[x+y*obj->tam_x]=-1;}
		}
		carga_estado+=carga_inc;
	}
	scr_init_reprintf (" > Predibujando terreno");
	/* DIBUJAMOS */
	obj->list=glGenLists(1);
	glDisable(GL_LIGHTING);
	glNewList(obj->list,GL_COMPILE);
		use_texture(texture);
		glPushMatrix();
		glTranslatef(-obj->ini_x,-obj->ini_y,-obj->ini_z);
		glRotatef(180, 0, 0, 1.0f);
		
		for (y=0;y<obj->tam_y-1; y++)
		{
			/* DIBUJO DEL TERRENO TEXTURIZADO */
			glEnable(GL_TEXTURE_2D);
			glBegin(GL_TRIANGLE_STRIP);
			for (x=0;x<obj->tam_x; x++)
			{
				color=obj->shadow[x+y*obj->tam_x];
				if(color>=0){glColor3f(color,color,color);}else{glColor3f(1.0,0,1.0);}
				if(y==0 && x==0){glColor3f(0.0,1,0.0);}
				/* Dibujamos el primer tri�ngulo */
				// Lo de obj->tam_x-x+1 es porque sale todo como en un espejo en las x's
				glTexCoord2i( (obj->tam_x-x+1 - half_x)*obj->scale, (y - half_y)  *obj->scale);
				glVertex3f  ( (obj->tam_x-x+1 - half_x)*obj->scale, (y - half_y)  *obj->scale, (obj->data[x+y*obj->tam_x]-obj->zero_h)*v_scale+v_add);
				
				color=obj->shadow[x+(y+1)*obj->tam_x];
				if(color>=0){glColor3f(color,color,color);}else{glColor3f(1.0,0,1.0);}
				if(y==0 && x==0){glColor3f(0.0,1,0.0);}
				/* Dibujamos el segundo tri�ngulo */
				glTexCoord2i( (obj->tam_x-x+1 - half_x)*obj->scale, (y+1 - half_y)*obj->scale);
				glVertex3f  ( (obj->tam_x-x+1 - half_x)*obj->scale, (y+1 - half_y)*obj->scale, (obj->data[x+(y+1)*obj->tam_x]-obj->zero_h)*v_scale+v_add);
			}
			glEnd();
			
			/* DIBUJO DE TRIANGULOS (wireframe) *//*
			glDisable(GL_TEXTURE_2D);
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			glBegin(GL_TRIANGLE_STRIP);
			glColor3f(0.0,0,0.0);
			for (x=0;x<obj->tam_x; x++)
			{
				glVertex3f  ( (obj->tam_x-x+1 - half_x)*obj->scale, (y - half_y)  *obj->scale, (obj->data[x+y*obj->tam_x]-obj->zero_h)*v_scale+v_add);
				glVertex3f  ( (obj->tam_x-x+1 - half_x)*obj->scale, (y+1 - half_y)*obj->scale, (obj->data[x+(y+1)*obj->tam_x]-obj->zero_h)*v_scale+v_add);
			}
			glEnd();
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			
			/* DIBUJO DE LAS NORMALES y la direcci�n del sol*//*
			glDisable(GL_TEXTURE_2D);
			glBegin(GL_LINES);
			for (x=obj->tam_x-1;x>=0; x--)
			{
				/* Normales *//*
				glColor3f(1.0,0,1.0);
				glVertex3f  ( (obj->tam_x-x+1                                - half_x)*obj->scale, (y- half_y)  *obj->scale, (obj->data[x+y*obj->tam_x]-obj->zero_h)*v_scale+v_add);
				glVertex3f  ( (obj->tam_x-x+1- half_x)*obj->scale + obj->normal[x+y*obj->tam_x].x*200, (y- half_y)  *obj->scale+obj->normal[x+y*obj->tam_x].y*200, (obj->data[x+y*obj->tam_x]-obj->zero_h)*v_scale+v_add+obj->normal[x+y*obj->tam_x].z*200);
				/* Rayos *//*
				glColor3f(0.0,1.0,0.0);
				glVertex3f  ( (obj->tam_x-x+1      - half_x)*obj->scale, (y - half_y)  *obj->scale, (obj->data[x+y*obj->tam_x]-obj->zero_h)*v_scale+v_add);
				glVertex3f  ( (obj->tam_x-x+1- half_x)*obj->scale-ray.x*200, (y- half_y)  *obj->scale-ray.y*200, (obj->data[x+y*obj->tam_x]-obj->zero_h)*v_scale-ray.z*200+v_add);//*/
			/*
			}
			glEnd();//*/
		}
		glPopMatrix();
	glEndList();
	scr_init_reprintf (" > Finalizado");
}
