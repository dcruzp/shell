#ifndef PARSER
#define PARSER

#include "shell_structures.h"

Command * Parse(char * text, char ** tokens);

//busca si existe algun signo
//ampersand en el comando
int Background(char * text);

//converitr los tokens en subcomandos y agregarlos
//a el comando a ejecutar
void MakeSubcmd(char ** tokens, int cmdCount, Command * cmd, subCommand * currentSubcmd);


#endif