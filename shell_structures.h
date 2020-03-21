#ifndef SHELL_STRUCTURES
#define SHELL_STRUCTURES

#include <stdlib.h>
#include <stdio.h>

typedef struct subCommand{

    //número de argumentos
    int argumentsCount;

    //instruccion
    char * cmd;
    //array de argumentos
    char * arguments;

}subCommand;

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



Command * initCommand(int subCmdCount, int background, char * comment, char * _stdin, char * _stdout, char * _stderr);

void CommandDestructor(Command * cmd, char * arguments);

subCommand * initSubCommand(char * cmd, char * Arguments);

void SubCommandestructor(subCommand * subcmd);

void insertSubcommand(Command * cmd, subCommand * subcmd);

void PrintCMD(Command * cmd);

#endif