#ifndef EXECUTE
#define EXECUTE
#include "shell_structures.h"

//Ejecuta una linea de comandos
int Execute(Command * cmd);

//Devuelve el file descriptor del que va a leer un subproceso
int GetInfd(subCommand * scmd);

//Devuelve el file descriptor en el que va a ecribirr un subproceso
int GetOutfd(subCommand * scmd);

//ejecuta un subproceso
int RunCMD(subCommand * scmd, int fdin, int fdout);

#endif