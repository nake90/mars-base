#include "parser.h"
#include <stdio.h>

#define PARSER_LINE_BUFFER_SIZE 1024

static void eliminar_espacios_iniciales(char *texto) /* Elimina los espacios iniciales de la línea */
{
	int a=0,b=0;
	while(texto[a]==' '){a++;}/* Buscamos el primer elemento sin espacios */
	if(a!=0)
	{
		while(texto[b+a]!='\0')
		{
			texto[b]=texto[b+a];
			b++;
		}
		texto[b]='\0';/* Nos aseguramos que acaba en 0 */
	}
}

static int linea_vacia(char* buffer) /* 1 si la línea está vacía o es un comentario */
{
	eliminar_espacios_iniciales(buffer);
	if(buffer[0]=='\n' || buffer[0]=='\0' || buffer[0]==';')return 1; /* Si es fin de línea o archivo (línea vacía) */
	if(buffer[0]=='/' && buffer[1]=='/')return 1; /* Si es un comentario pasamos de él */
	return 0;
}

static int str_cmp(const char* string1,const char* string2)
{
	int valor = 0;
	int pos=0;
	while (valor == 0 && string1[pos]!=0 && string2[pos]!=0)
	{
		valor=string1[pos]-string2[pos];
		pos++;
	}
	return valor;
}

/* Busca key en la lista de elementos cargados, retorna -1 si no lo encuentra */
static int parser_get_index(t_parse parse, const char *key)
{
	int i;
	for (i=0; i<parse.entradas; i++)
	{
		if (!str_cmp(key, parse.title[i])){return i;}
	}
	return -1;
}

static float str2float(const char *str)
{
      float valor=0;
      int negativo=1;
      float decimal=0.0;/*Necesito tipo float porque la división me sale entera y sin decimales con el tipo integer*/
      while (((*str>=0x30 && *str<=0x39) || *str==0x2E || *str==0x2C || *str==45) && *str!='\0') /*Entre 0 y 9 ó '.' ó ',' y no es el final*/
      {
            if (*str==45){negativo=-1;}
            else
            {
                if (*str>=0x30 && *str<=0x39)
                {
                   if (decimal==0)
                   {
                       valor*=10;
                       valor+=((*str)-0x30);
                   }else{
                         valor+=(*str-0x30)/decimal;
                         decimal*=10;
                   }
                }else{decimal=10;}
            }
            str++;
            
      }
      return valor*negativo;
}

int parse_open(t_parse* parse, char *ruta)
{
	FILE* file = fopen(ruta, "r");
	if(file==NULL){return -1;}
	
	char buffer[PARSER_LINE_BUFFER_SIZE];
	unsigned int lineas=0; /* número de entradas */
	
	/* Primera pasada -> Determinar el número de elementos a contar */
	while(fgets(buffer,PARSER_LINE_BUFFER_SIZE,file)!=NULL) /* fgets() lee una línea guardando el '\n'. Retorna NULL si EOF */
		if(!linea_vacia(buffer))lineas++;
	
	if(lineas<=0){return -2;}
	
	/* Creamos la lista de elementos */
	parse->title = malloc(sizeof(char*)*lineas);
	if(parse->title==NULL){return -3;}
	parse->value = malloc(sizeof(char*)*lineas);
	if(parse->value==NULL){free(parse->title); return -4;}
	
	/* Segunda pasada, leemos los datos y llenamos la matriz */
	rewind(file);
	char str[PARSER_LINE_BUFFER_SIZE];
	int actual=0;
	int len, len2, c1, c2;
	while(fgets(buffer,PARSER_LINE_BUFFER_SIZE,file)!=NULL)
	{
		if(!linea_vacia(buffer))
		{
			while(actual<lineas)/* Realmente no hace el bucle, es solo una comprobación */
			{
				/* NOMBRE */
				strcpy(str,buffer);
				len=strlen(str);
				/* Obtenemos el texto hasta la primera letra antes del '=' */
				c1=0;
				while(c1<len && str[c1] != '='){c1++;}
				if(c1==len){break;}/* La línea es errónea, no tiene un '=' */
				c2=c1; /* Guardamos el valor de la posición del '=' en c2 */
				/* Quitamos los espacios en blanco de antes del '=' */
				c1--;
				while(c1>0 && (str[c1] == ' ' || str[c1] == '\t')){c1--;}
				if(c1==0){break;}/* La línea es errónea, no tiene nombre */
				str[c1+1]='\0'; /* Borramos todo lo que no es el nombre del elemento */
				len2=strlen(str);
				parse->title[actual]=malloc(sizeof(char)*(len2+1));
				strcpy(parse->title[actual], str);
				
				/* VALOR */
				strcpy(str,buffer);
				c1=c2; /* Posición del '=' */
				/* Quitamos los espacios en blanco de después del '=' */
				c1++;
				while(c1<len && (str[c1] == ' ' || str[c1] == '\t')){c1++;}
				if(c1==len || str[c1]=='\n' || str[c1]=='\0' || str[c1]==';'){break;}/* La línea es errónea, no tiene valor */
				/* c1 es el índice al primer carácter después del '=' y los ' ' */
				
				/* Reusamos c2 para borrar el principio de str y el final (';','\n','//',...) */
				c2=0;
				while(c2+c1<len && str[c2+c1]!='\n' && str[c2+c1]!='\0' && str[c2+c1]!=';' && !(str[c2+c1]=='/' && str[c2+c1+1]=='/'))
				{
					str[c2]=str[c2+c1];
					c2++;
				}
				
				/* Tenemos en str el texto del valor que queremos, excepto porque puede que tenga espacios en blanco al final */
				c2--;
				while(c2>0 && (str[c2]==' ' || str[c2]=='\t'))c2--;
				str[c2+1]='\0';
				
				len2=strlen(str);
				parse->value[actual]=malloc(sizeof(char)*(len2+1));
				strcpy(parse->value[actual], str);
				
				
				actual++;
				break;
			}
		}
	}
	
	parse->entradas=actual; /* No "parse->entradas=lineas;", porque puede que haya habido errores al leer la línea */
	
	fclose(file);
	return 0;
}

void parse_close(t_parse* parse)
{
	int i;
	for(i=0; i<parse->entradas; i++)
	{
		free(parse->title[i]);
		parse->title[i]=NULL;
		free(parse->value[i]);
		parse->value[i]=NULL;
	}
	free(parse->title);
	parse->title=NULL;
	free(parse->value);
	parse->value=NULL;
	
	parse->entradas=0;
}

void parse_debug_print(const char *ruta, t_parse parse)
{
	int res;
	FILE *file = fopen(ruta,"a");
	fprintf(file,"Parser log:\n >> entradas: %i\n", parse.entradas);
	for(res=0; res<parse.entradas; res++)
	{
		fprintf(file, "\"%s\" = \"%s\"\n", parse.title[res], parse.value[res]);
	}
	fclose(file);
}

int parse_get_int(t_parse* parse, const char *entrada)
{
	int index = parser_get_index(*parse, entrada);
	if (index<0){return PARSE_NOT_FOUND;}
	
	return (int)str2float(parse->value[index]);
}

float parse_get_float(t_parse* parse, const char *entrada)
{
	int index = parser_get_index(*parse, entrada);
	if (index<0){return PARSE_NOT_FOUND;}
	
	return str2float(parse->value[index]);
}

char parse_get_char(t_parse* parse, const char *entrada)
{
	int index = parser_get_index(*parse, entrada);
	if (index<0){return PARSE_NOT_FOUND;}
	
	if (index<0){return PARSE_NOT_FOUND;}
	
	return parse->value[index][0];
}

int parse_get_str(t_parse* parse, const char *entrada, char* str)
{
	int index = parser_get_index(*parse, entrada);
	if (index<0){return PARSE_NOT_FOUND;}
	
	str_cpy(str,parse->value[index]);
	return strlen(parse->value[index]);
}
