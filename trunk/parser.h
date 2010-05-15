/*
	parser - Small utilities to parse config files
    By Alfonso Arbona Gimeno (nake)
    This is released as Public Domain code
*/

#ifndef PARSER_H
#define PARSER_H

/* Formato de los archivos:

//comentario
entrada = valor; //comentarios en la misma l�nea

El ';' es opcional.
Da igual si hay espacios en blanco, se omiten.
Las entradas pueden contener espacios en blanco excepto en los extremos ("valor 1" OK; " valor1 " MAL)
Los valores pueden ser texto, int, float, o char. Los tipo texto no usan comillas.


*** EJEMPLO ***

//Esto es un ejemplo de archivo para el parser
velocidad = 3.0;
//Se lee as�: float velocidad = parse_get_float(parse, "velocidad");

ayuda= /files/ayuda1.txt
//Se lee as�: int longitud_texto = parse_get_str(parse, "ayuda", dir_ayuda);

tecla ayuda = H
//Se lee as�: char tecla = parse_get_char(parse, "tecla ayuda");


NOTA: No se pueden leer cadenas de car�cteres que contienen '//' ya que se leen como comentarios
NOTA: PARSE_NOT_FOUND es -1, es decir, si cargas un valor que vale -1, se leer� c�mo si no se hubiese le�do
																				(aunque s� que se habr� leido)

EJEMPLO DE C�DIGO:

t_parse parse; // NO VALE USAR: t_parse *parse; Tiene que ser una variable real, no un puntero (si haces un malloc s�)
if(parse_open(&parse, "archivo.txt")==0)
{
	float velocidad = parse_get_float(&parse, "velocidad");
	
	char dir_ayuda[1024]; // Usar 1024 asegura que no se corta parte del nombre, pero puedes usar lo que creas conveniente
	int longitud_texto = parse_get_str(&parse, "ayuda", dir_ayuda);
	
	//parse_get_char realmente retorna la primera tecla de un string. Es decir si tenemos: "tecla ayuda = Hola" se leer� 'H'
	char tecla = parse_get_char(&parse, "tecla ayuda");
	
	parse_close(&parse);
}
else
{
// Error al cargar
}

*/

/* Tama�o de title y value en n�mero de car�cteres */

typedef struct
{
	char **title; /* Lista de t�tulos de las entradas */
	char **value; /* Lista de valores de las entradas (Tal como est�n escritas, despu�s se traducen a lo que se necesite) */
	unsigned int entradas; /* Cantidad de entradas que hay */
}t_parse;

int parse_open(t_parse* parse, char *ruta);
void parse_close(t_parse* parse);

void parse_debug_print(const char *ruta, t_parse parse); /* Guarda toda la informaci�n que tiene el parser en un
															archivo de texto en "ruta" */
															
/* Las funciones retornan PARSE_NOT_FOUND si no se encuentra la entrada buscada */
#define PARSE_NOT_FOUND -1
int parse_get_int(t_parse* parse, const char *entrada);
float parse_get_float(t_parse* parse, const char *entrada);
char parse_get_char(t_parse* parse, const char *entrada);
int parse_get_str(t_parse* parse, const char *entrada, char* str); /* str tiene que apuntar a una direcci�n v�lida */


#endif
