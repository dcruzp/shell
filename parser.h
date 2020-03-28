#ifndef PARSER
#define PARSER

#include "shell_structures.h"

//formatea la linea text para que sea más facil de procesar
int Parse(char * text, Command * cmd[]);

//busca si existe algun signo
//ampersand en el comando
int Background(char * text);



//quita los espacios de más en line,
//si hay más de un espacio deja solo uno.
char * removeOverSpace(char * line);

//quita los espacios al final de line.
char * removeSpaceAtBegining(char * line);

//quita los espacios al principio de line.
char * removeSpaceAtEnd(char * line);

//devuelve un mapa de enteros indicando a que tipo
//de token pertenece cada caracter.
int * token_map(char * text);

//si _char es un operador definido devuelve 1,
//en caso contrario devuelve 0.
int IsOperator(char _char);

//Concatena un caracter a un string.
void ConcatChar(char * str1, char _char);

#endif