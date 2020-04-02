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
    int argsC;

    //Cantidad de redirecciones de salida
    int outRcount;

    //Cantidad de redirecciones de entrada
    int inRcount;

    //cantidad de redirecciones de append
    int appRcount;

    //instruccion
    char * cmd;
    //array de argumentos
    char * args[64];
    //redirecciones de salida
    char * outR[64];
    //redirecciones de entrada
    char * inR[64];
    //redireciones de salida con append
    char * appR[64];

}subCommand;

/**
 * Un Command tiene varios subcomandos
 * que deben ser ejecutados en orden
 * mediante pipes, tambien contiene el
 * path de la entrada y de la salida en
 * caso de redireccionamiento.
*/
typedef struct Command{
    //cantidad de subcomandos
    int subCommandCount;
    //booleano para saber si hay procesos en el background
    int _background;
    //Comentarios si hay alguno
    char * comment;

    //texto del comando
    char * cmdtext;

    //array de subcomandos
    subCommand * commands;

}Command;


//inicializar la estructura Command.
Command * initCommand(char * comment);

/**
 * Destructor para el tipo Command
 * usar despues de trabajar con un Command* declardo
 * o devuelto por alguna función.
 */
void CommandDestructor(Command * cmd);

//inicializa un subcomand.
subCommand * initSubCommand(char * cmd);

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

//Agregar argumentos a un subcomando
void insertArg(subCommand * scmd, char * arg);

//Agregar el path de un redirecionamiento de entrada a un subcomando
void insertInRedir(subCommand * scmd ,char * inputRedir);

//Agregar el path de un redirecionamiento de salida a un subcomando
void insertOutRedir(subCommand * scmd ,char * outputRedir);

//Agregar el path de un redirecionamiento de entrada tipo append a un subcomando
void insertAppendRedir(subCommand * scmd ,char * appendRedir);

#endif