/**
 * Parser para transformar la entrada del shell
 * en tokens y devolverlos al shell para que los
 * procese
**/
#include "parser.h"
#include "shell_structures.h"
#include <stdio.h>
#include <string.h>


Command * Parse(char * text, char ** tokens)
{

    Command * cmd;
    subCommand * currentSubcmd;
    int cmdCount = 0;
    int background = 0;    
    char * _stdin, *_stdout, *_stderr, *comment;
    

    char * str1, *token;
    int j = 0;
    int i = 0;

    char * pipeDelimiter = "|";
    char * commentDelimiter = "#";
    char * inDelimiter = "<";
    char * outDelimiter = ">";
    char * ampersandDelimiter = "&";

    //buscar comentarios al final del comando
    for (str1 = text; j < 2; j++, str1 = NULL)
    {
        token = strtok(str1, commentDelimiter);
        if(token == NULL){ break;}
        if(j = 0)
        {
            text = token;
            continue;
        }
        comment = token;
    }
    //separar la entrada por pipes
    for(str1 = text; ; cmdCount++, str1 = NULL)
    {
        token = strtok(str1, pipeDelimiter);

        if(token == NULL){ break;}

        tokens[cmdCount] = token;
    }

    //no habian comandos para ejecutar
    if(cmdCount == 0)
    {
        perror("No comand to execute");
    }


    //Buscar si hay redirección de salida
    //que debe acompañar al último comando
    for (str1 = tokens[cmdCount - 1], i = 0; ;i++, str1 = NULL)
    {
        token = strtok(str1, outDelimiter);
        if(token == NULL){ break;}

        if(i == 1)
        {
            _stdout = token;
            break;
        }
        if(i == 0){ tokens[cmdCount - 1] = token;}
    }
    
    //Buscar si hay redirección de entrada
    //que debe acompañar al primer comando
    for (i = 0, str1 = tokens[0]; ;i++, str1 = NULL)
    {
        token = strtok(str1, inDelimiter);
        if(token == NULL){ break;}

        if(i == 1)
        {
            _stdin = token;
            break;
        }
        if (i == 0){ tokens[0] = token;}

    }

    //buscar ampersand para saber si el comando debe correr en
    //el background
    background = Background(tokens[cmdCount - 1]);
    if(background)
    {
        for (i = 0, str1 = tokens[cmdCount - 1]; ;i++, str1 = NULL )
        {
            token = strtok(str1, ampersandDelimiter);

            if(token == NULL){ break;}

            if(i == 0){ tokens[cmdCount - 1] = token;}
        }
    }

    cmd = initCommand(cmdCount, background, comment, _stdin, _stdout, NULL);
    MakeSubcmd(tokens, cmdCount, cmd, currentSubcmd);

    PrintCMD(cmd);

    return cmd;
}


//busca si existe algun signo
//ampersand en el comando
int Background(char * text)
{
    int count = strlen(text);
    for(int i = count; i >=0; i--)
    {
        if(text[i] == '&')
        {
            return 1;
        }
    }

    return 0;
}


//converitr los tokens en subcomandos y agregarlos
//a el comando a ejecutar
void MakeSubcmd(char ** tokens, int cmdCount, Command * cmd, subCommand * currentSubcmd)
{

    char * str1;
    char * delimiter = " ";
    int j = 0;
    for (int i = 0; i < cmdCount; i++)
    {
        char * _cmd, *args, *token;
        for (j = 0, str1 = tokens[i]; ;j++, str1 = NULL)
        {
            token = strtok(str1, delimiter);
            if(token == NULL){ break;}

            if(j == 0)
            {
                 _cmd = token;
                 continue;
            }

            if(j == 1){ args = token;}
            
        }

        
        currentSubcmd = initSubCommand( _cmd, args);
        insertSubcommand(cmd, currentSubcmd);
        
        SubCommandestructor(currentSubcmd);
    }
    
}


