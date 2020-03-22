#ifndef SHELL_STRUCTURES
#define SHELL_STRUCTURES

#include <stdlib.h>
#include <stdio.h>

/**
 * Aquí guardamos los datos de un comando
 * atómico, o sea que solo tiene un comando
 * para ejecutar.
*/
typedef struct subCommand{

    //número de argumentos
    int argumentsCount;

    //instruccion
    char * cmd;
    //array de argumentos
    char * arguments;

}subCommand;

/**
 * Un Command tiene varios subcomandos
 * que deben ser ejecutados en orden
 * mediante pipes, tambien contiene el
 * path de la entrada y de la salida en
 * caso de redireccionamiento.
*/
typedef struct Command{
    //para saber donde insertar un subcomando
    int currentSubcmd;
    //cantidad de subcomandos
    int subCommandCount;
    //booleano para saber si hay procesos en el background
    int _background;
    //fichero de entrada estandar
    char * _stdin;
    //fichero de salida estandar
    char * _stdout;
    //fichero de salida estandar de errores
    char * _stderr;
    //Comentarios si hay alguno
    char * comment;

    //array de subcomandos
    subCommand * commands;


}Command;


//inicializar la estructura Command.
Command * initCommand(int subCmdCount, int background, char * comment, char * _stdin, char * _stdout, char * _stderr);

/**
 * Destructor para el tipo Command
 * usar despues de trabajar con un Command* declardo
 * o devuelto por alguna función.
 */
void CommandDestructor(Command * cmd);

//inicializa un subcomand.
subCommand * initSubCommand(char * cmd, char * Arguments);

/**
 * Destructor para el tipo subCommand
 * usar despues de trabajar con un subCommand* declardo
 * o devuelto por alguna función.
 */
void SubCommandestructor(subCommand * subcmd);

/**
 * agrega un subCommand a un Command
 * WARNING: tener en cuenta que un Command
 * solo acepta un numero limitado de subcomandos
 * de lo contrario lanzauna advertencia.
*/
void insertSubcommand(Command * cmd, subCommand * subcmd);


/**
 * Imprime en la salida estandar datos
 * de los comandos guardados en un
 * Command*
*/
void PrintCMD(Command * cmd);

#endif