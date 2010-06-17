/*
	parser - Small utilities to parse config files
    By Alfonso Arbona Gimeno (nake)
    This is released as Public Domain code
*/

#ifndef PARSER_H
#define PARSER_H

/* Formato de los archivos:

//comentario
entrada = valor; //comentarios en la misma línea

El ';' es opcional.
Da igual si hay espacios en blanco, se omiten.
Las entradas pueden contener espacios en blanco excepto en los extremos ("valor 1" OK; " valor1 " MAL)
Los valores pueden ser texto, int, float, o char. Los tipo texto no usan comillas.


*** EJEMPLO ***

//Esto es un ejemplo de archivo para el parser
velocidad = 3.0;
//Se lee así: float velocidad = parse_get_float(parse, "velocidad");

ayuda= /files/ayuda1.txt
//Se lee así: int longitud_texto = parse_get_str(parse, "ayuda", dir_ayuda);

tecla ayuda = H
//Se lee así: char tecla = parse_get_char(parse, "tecla ayuda");


NOTA: No se pueden leer cadenas de carácteres que contienen '//' ya que se leen como comentarios
NOTA: PARSE_NOT_FOUND es -1, es decir, si cargas un valor que vale -1, se leerá cómo si no se hubiese leído
																				(aunque sí que se habrá leido)

EJEMPLO DE CÓDIGO:

t_parse parse; // NO VALE USAR: t_parse *parse; Tiene que ser una variable real, no un puntero (si haces un malloc sí)
if(parse_open(&parse, "archivo.txt")==0)
{
	float velocidad = parse_get_float(&parse, "velocidad");

	char dir_ayuda[1024]; // Usar 1024 asegura que no se corta parte del nombre, pero puedes usar lo que creas conveniente
	int longitud_texto = parse_get_str(&parse, "ayuda", dir_ayuda);

	//parse_get_char realmente retorna la primera tecla de un string. Es decir si tenemos: "tecla ayuda = Hola" se leerá 'H'
	char tecla = parse_get_char(&parse, "tecla ayuda");

	parse_close(&parse);
}
else
{
// Error al cargar
}

*/

/* Tamaño de title y value en número de carácteres */

typedef struct
{
	char **title; /* Lista de títulos de las entradas */
	char **value; /* Lista de valores de las entradas (Tal como están escritas, después se traducen a lo que se necesite) */
	unsigned int entradas; /* Cantidad de entradas que hay */
}t_parse;

int parse_open(t_parse* parse, char *ruta);
void parse_close(t_parse* parse);

void parse_debug_print(const char *ruta, t_parse parse); /* Guarda toda la información que tiene el parser en un
															archivo de texto en "ruta" */

/* Las funciones retornan PARSE_NOT_FOUND si no se encuentra la entrada buscada */
#define PARSE_NOT_FOUND -1
int parse_get_int(t_parse* parse, const char *entrada);
float parse_get_float(t_parse* parse, const char *entrada);
char parse_get_char(t_parse* parse, const char *entrada);
int parse_get_str(t_parse* parse, const char *entrada, char* str); /* str tiene que apuntar a una dirección válida */


#endif
