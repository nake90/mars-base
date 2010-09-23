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

/** \file entities.h
 * \brief Creación y control de objetos asociados a entidades y la ejeccución de scripts relacionados.
 * Básicamente este archivo define todos los elementos necesarios para cargar archivos LUA que definen
 * la forma en la que se comportan las distintas entidades. Estas entidades son objetos con elementos
 * dinámicos como generadores, puertas, ...
 * \author Alfonso Arbona Gimeno
*/

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>

#include "display.h"
#include "objetos.h"
#include "entities.h"

typedef struct s_entity
{
	//unsigned char lua_id; /*!< Id al correspondiente instance de LUA */
	//char name[ENTITY_NAME_MAXLEN]; /*!< Nombre de la entidad */
	unsigned int obj_base; /*!< Objeto al que está enlazado */
	lua_State *lua_vm; /*!< Máquina virtual de LUA */
}t_entity;

t_entity **lista_entity = NULL; /*!< Lista de entidades */
int lista_entities = 0; /*!< Número de entidades */


char **lista_entity_names = NULL; /*!< Lista de nombres de entidades */
int lista_entities_names = 0;

//lua_State **lua_vm = NULL; /*!< Lista de máquinas virtuales LUA */
//int lua_instances = 0;


static
void my_lua_push_object(lua_State *L, unsigned int id)
{
	lua_newtable(L);

	lua_pushliteral(L, "id"); /* Index */
	lua_pushvalue(L, id); /* Valor */
	lua_rawset(L, -3); /* Lo guardamos en la tabla */

	lua_pushliteral(L, "name"); /* Index */
	lua_pushstring(L, lista_objeto_base[id]->name); /* Valor */
	lua_rawset(L, -3); /* Lo guardamos en la tabla */

	lua_pushliteral(L, "model"); /* Index */
	lua_pushstring(L, lista_objeto_base[id]->modelo->name); /* Valor */
	lua_rawset(L, -3); /* Lo guardamos en la tabla */

	lua_pushliteral(L, "pos"); /* Index */
		lua_newtable(L);
		lua_pushliteral(L, "x"); /* Index */
		lua_pushnumber(L, lista_objeto_base[id]->pos.x); /* Valor */
		lua_settable(L, -3);

		lua_pushliteral(L, "y"); /* Index */
		lua_pushnumber(L, lista_objeto_base[id]->pos.y); /* Valor */
		lua_settable(L, -3);

		lua_pushliteral(L, "z"); /* Index */
		lua_pushnumber(L, lista_objeto_base[id]->pos.z); /* Valor */
		lua_settable(L, -3);
	lua_rawset(L, -3); /* Lo guardamos en la tabla */

	lua_pushliteral(L, "rot"); /* Index */
		lua_newtable(L);
		lua_pushliteral(L, "pitch"); /* Index */
		lua_pushnumber(L, lista_objeto_base[id]->rot.x); /* Valor */
		lua_settable(L, -3);

		lua_pushliteral(L, "yaw"); /* Index */
		lua_pushnumber(L, lista_objeto_base[id]->rot.y); /* Valor */
		lua_settable(L, -3);

		lua_pushliteral(L, "roll"); /* Index */
		lua_pushnumber(L, lista_objeto_base[id]->rot.z); /* Valor */
		lua_settable(L, -3);
	lua_rawset(L, -3); /* Lo guardamos en la tabla */

	lua_pushliteral(L, "sq"); /* Index */
		lua_newtable(L);
		lua_pushliteral(L, "left"); /* Index */
		lua_pushnumber(L, lista_objeto_base[id]->sq_l); /* Valor */
		lua_settable(L, -3);

		lua_pushliteral(L, "right"); /* Index */
		lua_pushnumber(L, lista_objeto_base[id]->sq_r); /* Valor */
		lua_settable(L, -3);

		lua_pushliteral(L, "top"); /* Index */
		lua_pushnumber(L, lista_objeto_base[id]->sq_t); /* Valor */
		lua_settable(L, -3);

		lua_pushliteral(L, "bottom"); /* Index */
		lua_pushnumber(L, lista_objeto_base[id]->sq_b); /* Valor */
		lua_settable(L, -3);
	lua_rawset(L, -3); /* Lo guardamos en la tabla */

	lua_pushliteral(L, "conx_qty"); /* Index */
	lua_pushvalue(L, lista_objeto_base[id]->conx_qty); /* Valor */
	lua_rawset(L, -3); /* Lo guardamos en la tabla */

	//! POR IMPLEMENTAR: Matrices de conexiones...

	lua_pushliteral(L, "reparar"); /* Index */
	lua_pushnumber(L, lista_objeto_base[id]->reparar); /* Valor */
	lua_rawset(L, -3); /* Lo guardamos en la tabla */

	lua_pushliteral(L, "node_data"); /* Index */
		lua_newtable(L);
		lua_pushliteral(L, "gases"); /* Index */
			lua_newtable(L);
			lua_pushliteral(L, "CO2");
			lua_pushnumber(L, lista_objeto_base[id]->node_data.gases.CO2);
			lua_settable(L, -3);
			lua_pushliteral(L, "N2");
			lua_pushnumber(L, lista_objeto_base[id]->node_data.gases.N2);
			lua_settable(L, -3);
			lua_pushliteral(L, "Ar");
			lua_pushnumber(L, lista_objeto_base[id]->node_data.gases.Ar);
			lua_settable(L, -3);
			lua_pushliteral(L, "O2");
			lua_pushnumber(L, lista_objeto_base[id]->node_data.gases.O2);
			lua_settable(L, -3);
			lua_pushliteral(L, "CO");
			lua_pushnumber(L, lista_objeto_base[id]->node_data.gases.CO);
			lua_settable(L, -3);
			lua_pushliteral(L, "H2O");
			lua_pushnumber(L, lista_objeto_base[id]->node_data.gases.H2O);
			lua_settable(L, -3);
		lua_settable(L, -3);

		lua_pushliteral(L, "volumen"); /* Index */
		lua_pushnumber(L, lista_objeto_base[id]->node_data.volumen); /* Valor */
		lua_settable(L, -3);

		lua_pushliteral(L, "temperatura"); /* Index */
		lua_pushnumber(L, lista_objeto_base[id]->node_data.temperatura); /* Valor */
		lua_settable(L, -3);
	lua_rawset(L, -3); /* Lo guardamos en la tabla */

	lua_pushliteral(L, "selec"); /* Index */
	lua_pushboolean(L, lista_objeto_base[id]->selec); /* Valor */
	lua_rawset(L, -3); /* Lo guardamos en la tabla */
}

static
void my_lua_update_object(lua_State *L, unsigned int id)
{
	lua_pushstring(L, "name"); // Key que queremos obtener
	lua_gettable(L, -2); // Le decimos que lo busque en la tabla que hay en el stack
	str_cpyl(lista_objeto_base[id]->name, 256, lua_tostring(L, -1));
	lua_pop(L, 1); // Quitamos el elemento del stack

	lua_pushstring(L, "pos");
	lua_gettable(L, -2);
		lua_pushstring(L, "x");
		lua_gettable(L, -2);
		lista_objeto_base[id]->pos.x = lua_tonumber(L, -1);
		lua_pop(L, 1);
		lua_pushstring(L, "y");
		lua_gettable(L, -2);
		lista_objeto_base[id]->pos.y = lua_tonumber(L, -1);
		lua_pop(L, 1);
		lua_pushstring(L, "z");
		lua_gettable(L, -2);
		lista_objeto_base[id]->pos.z = lua_tonumber(L, -1);
		lua_pop(L, 1);
	lua_pop(L, 1);

	lua_pushstring(L, "rot");
	lua_gettable(L, -2);
		lua_pushstring(L, "pitch");
		lua_gettable(L, -2);
		lista_objeto_base[id]->rot.x = lua_tonumber(L, -1);
		lua_pop(L, 1);
		lua_pushstring(L, "yaw");
		lua_gettable(L, -2);
		lista_objeto_base[id]->rot.y = lua_tonumber(L, -1);
		lua_pop(L, 1);
		lua_pushstring(L, "roll");
		lua_gettable(L, -2);
		lista_objeto_base[id]->rot.z = lua_tonumber(L, -1);
		lua_pop(L, 1);
	lua_pop(L, 1);

	lua_pushstring(L, "sq");
	lua_gettable(L, -2);
		lua_pushstring(L, "left");
		lua_gettable(L, -2);
		lista_objeto_base[id]->sq_l = lua_tonumber(L, -1);
		lua_pop(L, 1);
		lua_pushstring(L, "right");
		lua_gettable(L, -2);
		lista_objeto_base[id]->sq_r = lua_tonumber(L, -1);
		lua_pop(L, 1);
		lua_pushstring(L, "top");
		lua_gettable(L, -2);
		lista_objeto_base[id]->sq_t = lua_tonumber(L, -1);
		lua_pop(L, 1);
		lua_pushstring(L, "bottom");
		lua_gettable(L, -2);
		lista_objeto_base[id]->sq_b = lua_tonumber(L, -1);
		lua_pop(L, 1);
	lua_pop(L, 1);

	//! POR IMPLEMENTAR: Matrices de conexiones...

	lua_pushstring(L, "reparar");
	lua_gettable(L, -2);
	lista_objeto_base[id]->reparar = lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_pushstring(L, "node_data");
	lua_gettable(L, -2);
		lua_pushstring(L, "gases");
		lua_gettable(L, -2);
			lua_pushstring(L, "CO2");
			lua_gettable(L, -2);
			lista_objeto_base[id]->node_data.gases.CO2 = lua_tonumber(L, -1);
			lua_pop(L, 1);
			lua_pushstring(L, "N2");
			lua_gettable(L, -2);
			lista_objeto_base[id]->node_data.gases.N2 = lua_tonumber(L, -1);
			lua_pop(L, 1);
			lua_pushstring(L, "Ar");
			lua_gettable(L, -2);
			lista_objeto_base[id]->node_data.gases.Ar = lua_tonumber(L, -1);
			lua_pop(L, 1);
			lua_pushstring(L, "O2");
			lua_gettable(L, -2);
			lista_objeto_base[id]->node_data.gases.O2 = lua_tonumber(L, -1);
			lua_pop(L, 1);
			lua_pushstring(L, "CO");
			lua_gettable(L, -2);
			lista_objeto_base[id]->node_data.gases.CO = lua_tonumber(L, -1);
			lua_pop(L, 1);
			lua_pushstring(L, "H2O");
			lua_gettable(L, -2);
			lista_objeto_base[id]->node_data.gases.H2O = lua_tonumber(L, -1);
			lua_pop(L, 1);
		lua_pop(L, 1);
		lua_pushstring(L, "volumen");
		lua_gettable(L, -2);
		lista_objeto_base[id]->node_data.volumen = lua_tonumber(L, -1);
		lua_pop(L, 1);
		lua_pushstring(L, "temperatura");
		lua_gettable(L, -2);
		lista_objeto_base[id]->node_data.temperatura = lua_tonumber(L, -1);
		lua_pop(L, 1);
	lua_pop(L, 1);

	lua_pushstring(L, "selec");
	lua_gettable(L, -2);
	lista_objeto_base[id]->selec = lua_toboolean(L, -1);
	lua_pop(L, 1);

	lua_pop(L, 1); // Quitamos la tabla del stack
}


static
int my_lua_message(lua_State *L)
{
	int argc = lua_gettop(L);

	if(argc != 1){lua_settop( L, 0 ); return 0;}

	//char buffer[2048];

	message_printf(lua_tostring (L, -1));
	debug_printf("Mensaje desde LUA: \"%s\"\n",lua_tostring (L, -1));

	lua_settop( L, 0 );
	return 0;
}

static
int my_lua_debug_print(lua_State *L)
{
	int argc = lua_gettop(L);

	if(argc != 1){lua_settop( L, 0 ); return 0;}

	//char buffer[2048];

	debug_printf("%s\n",lua_tostring (L, -1));

	lua_settop( L, 0 );
	return 0;
}

static
int my_lua_get_object_info(lua_State *L)
{
	int argc = lua_gettop(L);

	if(argc != 1){lua_settop( L, 0 ); return 0;}
	int id = lua_tointeger(L, -1);
	lua_settop( L, 0 );
	if(id < 0 || id >= lista_objetos_base){return 0;}

	my_lua_push_object(L, id);

	return 1;
}

static
int my_lua_get_object_number(lua_State *L)
{
	lua_settop( L, 0 );
	lua_pushvalue(L, lista_objetos_base);

	return 1;
}

static
int my_lua_kill_object(lua_State *L)
{
	int id = lua_tointeger(L, -1);
	lua_settop( L, 0 );
	if(id < 0 || id >= lista_objetos_base){lua_pushboolean(L, 0);return 1;}
	lista_base_borrar_elemento(id);
	lua_pushboolean(L, 1);
	return 1;
}

static
void my_lua_lib_register(lua_State *L)
{
	lua_register(L, "print", my_lua_message);
	lua_register(L, "message", my_lua_message);
	lua_register(L, "debug_print", my_lua_debug_print);
	lua_register(L, "obj_qty", my_lua_get_object_number);
	lua_register(L, "obj_info", my_lua_get_object_info);
	lua_register(L, "obj_kill", my_lua_kill_object);
}

void entity_execf(unsigned int id, const char* function_name)
{
	lua_getglobal(lista_entity[id]->lua_vm, function_name);

	my_lua_push_object(lista_entity[id]->lua_vm,lista_entity[id]->obj_base);
	/* 1 argumento, 1 retorno, 0 debug */
	if(lua_pcall(lista_entity[id]->lua_vm,1,1,0)!=0)
	{
		debug_printf("LUA error en %s(): %s",function_name,lua_tostring(lista_entity[id]->lua_vm, -1));
		message_printf("LUA error en update(): %s",lua_tostring(lista_entity[id]->lua_vm, -1));
		entity_kill(id);
		return;
	}
	my_lua_update_object(lista_entity[id]->lua_vm,lista_entity[id]->obj_base);
}

void entity_exec_update(unsigned int id, double time_elapsed)
{
	lua_getglobal(lista_entity[id]->lua_vm,"update");

	my_lua_push_object(lista_entity[id]->lua_vm,lista_entity[id]->obj_base);
	lua_pushnumber(lista_entity[id]->lua_vm, (lua_Number)time_elapsed);
	/* 1 argumento, 1 retorno, 0 debug */
	if(lua_pcall(lista_entity[id]->lua_vm,2,1,0)!=0)
	{
		debug_printf("LUA error en update(): %s",lua_tostring(lista_entity[id]->lua_vm, -1));
		message_printf("LUA error en update(): %s",lua_tostring(lista_entity[id]->lua_vm, -1));
		entity_kill(id);
		return;
	}
	my_lua_update_object(lista_entity[id]->lua_vm,lista_entity[id]->obj_base);
}

#ifdef WINDOWS
int entities_get_names_dir(const char *dir)
{
    struct _finddata_t datos; // Datos del archivo o carpeta encontrados
    char dir_buffer[1024];
    long handle;

    str_cpyl(dir_buffer,1023,dir);
    str_append(dir_buffer,"/*"); // Indicamos que queremos buscar en toda la carpeta

    handle = _findfirst (dir_buffer, &datos);
    if(handle==-1)
    {
        debug_printf("Error buscando entities en %s\n", dir_buffer);
        return -1;
    }
    do
    {
        if( !(datos.attrib & _A_SUBDIR) && str_ext_cmp(datos.name,"lua")==1) // Si es de tipo lua y no es un directorio
        {
            lista_entity_names = realloc(lista_entity_names, sizeof(char*)*(lista_entities_names+1));
            if(!lista_entity_names)
            {
            	debug_printf("Error en el realloc de la lista de nombres de entidades. (%i bytes)\n",sizeof(char*)*(lista_entities_names+1));
            	return -10;
            }
            lista_entity_names[lista_entities_names] = malloc(sizeof(char) * (str_len(datos.name)+1));
            if(!lista_entity_names)
            {
            	debug_printf("Error en el malloc del elemento de lista de nombres de entidades. (%i bytes)\n",sizeof(char) * (str_len(datos.name)+1));
            	return -11;
            }
            str_cpy(lista_entity_names[lista_entities_names],datos.name);
            lista_entities_names++;
        }
    }
    while(_findnext(handle, &datos)!=-1);

    _findclose(handle);
    return 0;
}
#endif
#ifdef LINUX
int entities_get_names_dir(const char *dir)
{
    register struct dirent *datos; // Datos del archivo o carpeta encontrados. Register para que sea más rápido y cómodo para la CPU
    char dir_buffer[1024];
    register DIR *handle;

    str_cpyl(dir_buffer,1023,dir);
    str_append(dir_buffer,"/"); // Indicamos que queremos buscar en toda la carpeta

    if(handle==NULL)
    {
        debug_printf(TL_ERR_MODEL_DIR,dir_buffer);
        return -1;
    }

    while(datos=readdir(handle))
    {
        if( !(datos->d_type == DT_DIR) && str_ext_cmp(datos->d_name,"lua")==1) // Si es de tipo lua y no es un directorio
        {
            lista_entity_names = realloc(lista_entity_names, sizeof(char*)*(lista_entities_names+1));
            if(!lista_entity_names)
            {
            	debug_printf("Error en el realloc de la lista de nombres de entidades. (%i bytes)\n",sizeof(char*)*(lista_entities_names+1));
            	return -10;
            }
            lista_entity_names[lista_entities_names] = malloc(sizeof(char) * (str_len(datos->d_name)+1));
            if(!lista_entity_names)
            {
            	debug_printf("Error en el malloc del elemento de lista de nombres de entidades. (%i bytes)\n",sizeof(char) * (str_len(datos->d_name)+1));
            	return -11;
            }
            str_cpy(lista_entity_names[lista_entities_names],datos->d_name);
            lista_entities_names++;
        }
    }
    closedir(handle);
    return 0;
}
#endif

int entity_init(void) // TODO: Añadir todo a LANG
{
	ifdebug(DEBUG_INFO){debug_printf("Iniciando las entidades\n");}
	lista_entity = NULL;
	lista_entities = 0;

	//! Buscamos archivos
	if(entities_get_names_dir("data/entities")!=0)
	{
		debug_printf("Error al buscar los archivos de script.\n");
	}

	//! Inicializamos lua
	ifdebug(DEBUG_DEBUG){debug_printf("\tIniciando LUA\n");}
	//lua_instances = 0;

	return 0;
}

int entity_list_load(const char* filename, int object_id)
{
	ifdebug(DEBUG_INFO) debug_printf("Creando entidad\n");
	char nombre[1024];

	ifdebug(DEBUG_DEBUG) debug_printf("Entidad buscada: \"%s\"\n",filename);


	str_cpyl(nombre, 1024, filename);
	str_ruta_get_filename(nombre);
	str_ext_back(nombre);

	char _filename[2048];
	str_cpy(_filename,"data/entities/");
	str_append(_filename,filename);

	ifdebug(DEBUG_VERBOSE) debug_printf("Buscando entidad en \"%s\"\n",_filename);

	FILE* file = fopen(_filename,"r");
	if(!file){debug_printf("Error, archivo lua \"%s\" no encontrado.\n",_filename); return -1;}
	fclose(file);

	lista_entity = realloc(lista_entity, sizeof(t_entity*) * (lista_entities + 1));
	if(!lista_entity){debug_printf("Error al hacer el realloc de lista_entity. (%i entidades = %ibytes)\n",lista_entities+1, sizeof(t_entity*) * (lista_entities + 1)); return -1;}

	lista_entity[lista_entities] = malloc(sizeof(t_entity));
	if(!lista_entity[lista_entities]){debug_printf("Error al hacer el malloc del entity %s.\n",nombre); return -1;}

	//str_cpyl(lista_entity[lista_entities]->name, ENTITY_NAME_MAXLEN, nombre);

	lista_entity[lista_entities]->obj_base = object_id;

	lista_entity[lista_entities]->lua_vm = lua_open();
	if (!lista_entity[lista_entities]->lua_vm){debug_printf("Error al crear la máquina virtual de LUA %i \"%s\"\n",lista_entities,nombre); return -2;}

	//luaL_openlibs(lista_entity[lista_entities]->lua_vm);
	luaopen_base(lista_entity[lista_entities]->lua_vm);
	luaopen_table(lista_entity[lista_entities]->lua_vm);
	luaopen_string(lista_entity[lista_entities]->lua_vm);
	luaopen_math(lista_entity[lista_entities]->lua_vm);

	my_lua_lib_register(lista_entity[lista_entities]->lua_vm);

	if(luaL_dofile(lista_entity[lista_entities]->lua_vm,_filename)!=0)
	{
		message_printf("LUA error al cargar el archivo %s\nError: %s",_filename,lua_tostring(lista_entity[lista_entities]->lua_vm, -1));
		debug_printf("LUA error al cargar el archivo %s\nError: %s",_filename,lua_tostring(lista_entity[lista_entities]->lua_vm, -1));
		lua_close(lista_entity[lista_entities]->lua_vm);
		free(lista_entity[lista_entities]);
		lista_entity[lista_entities]=NULL;
		return -10;
	}

	lua_getglobal(lista_entity[lista_entities]->lua_vm,"create");

	my_lua_push_object(lista_entity[lista_entities]->lua_vm, lista_entity[lista_entities]->obj_base);
	/* 1 argumento, 1 retorno, 0 debug */
	if(lua_pcall(lista_entity[lista_entities]->lua_vm,1,1,0)!=0)
	{
		message_printf("LUA error en init(): %s",lua_tostring(lista_entity[lista_entities]->lua_vm, -1));
		debug_printf("LUA error en init(): %s",lua_tostring(lista_entity[lista_entities]->lua_vm, -1));
		lua_close(lista_entity[lista_entities]->lua_vm);
		free(lista_entity[lista_entities]);
		lista_entity[lista_entities]=NULL;
		return -11;
	}
	my_lua_update_object(lista_entity[lista_entities]->lua_vm, lista_entity[lista_entities]->obj_base);
	//"table" <- message_printf(lua_typename(lista_entity[lista_entities]->lua_vm,lua_type(lista_entity[lista_entities]->lua_vm,-1)));

	lista_entities++;
	return 0;
}

int entity_id_from_object(int id)
{
	int i;
	for(i=0;i<lista_entities;i++)
	{
		if(lista_entity[i]!=NULL && lista_entity[i]->obj_base == id)
			return i;
	}
	return -1;
}

void entity_kill(int id)
{
    ifdebug(DEBUG_INFO)debug_printf("Borrando el entity %i\n",id);
	lua_close(lista_entity[id]->lua_vm);
	free(lista_entity[id]);
	lista_entity[id]=NULL;
}

void entity_list_unload(void)
{
    ifdebug(DEBUG_INFO)debug_printf("Borrando %i entidades\n",lista_entities);
    lista_entities--;
    while(lista_entities>=0)
    {
		lua_close(lista_entity[lista_entities]->lua_vm);
        free(lista_entity[lista_entities]);
        lista_entities--;
    }
    free(lista_entity);
    lista_entities=0;
    lista_entity=NULL;
}
