#include "shell_structures.h"


Command * initCommand(char * comment)
{
    Command * cmd = (Command*)malloc(sizeof(Command));
    cmd->subCommandCount = 0;
    cmd->_background = 0;
    cmd->comment = comment;
    cmd->commands = (subCommand*)malloc(sizeof(subCommand));
    return cmd;
}

void CommandDestructor(Command * cmd)
{
    free(cmd->commands);
    free(cmd);
}

subCommand * initSubCommand(char * cmd)
{
    subCommand * subcmd = (subCommand*)malloc(sizeof(subCommand));
    subcmd->cmd = cmd;
    subcmd->appRcount = 0;
    subcmd->argsC = 0;
    subcmd->inRcount = 0;
    subcmd->outRcount = 0;    
    return subcmd;
}

void SubCommandestructor(subCommand * subcmd)
{
    free(subcmd);
}


void insertSubcommand(Command * cmd, subCommand * subcmd)
{
    int index = cmd->subCommandCount;
    cmd->commands = realloc(cmd->commands, index+1);
    cmd->commands[index] = *subcmd;
    cmd->subCommandCount++;
}

void PrintCMD(Command * cmd)
{
    if(cmd == NULL){
        printf("No hay comandos para ejeutar");
    }
    int count = cmd->subCommandCount;

    for (int i = 0; i < count; i++)
    {
        subCommand aux = cmd->commands[i];
        char * _cmd = aux.cmd;
        /*char * args = aux.args;
        if( args == NULL)
        {
            args = "NULL";
        }
        printf("\nComand #%d:\n-cmd: %s\n-args: %s\n", i, _cmd, args);
        */
    }
    
}


void insertArg(subCommand * scmd, char * arg)
{
    if(scmd->argsC >= 63)
    {
        printf("To many arguments. Are they necesary?\n");
        exit(1);
    }
    
    scmd->args[scmd->argsC] = arg;
    scmd->argsC++;
    scmd->args[scmd->argsC] = NULL;
}

void insertInRedir(subCommand * scmd ,char * inputRedir)
{
    if(scmd->inRcount >= 63)
    {
        printf("To many input redirections. Are they necesary?\n");
        exit(1);
    }
    
    scmd->inR[scmd->inRcount] = inputRedir;
    scmd->inRcount++;
    scmd->inR[scmd->inRcount] = NULL;
}

void insertOutRedir(subCommand * scmd ,char * outputRedir)
{
     if(scmd->outRcount >= 63)
    {
        printf("To many output redirections. Are they necesary?\n");
        exit(1);
    }
    
    scmd->outR[scmd->outRcount] = outputRedir;
    scmd->outRcount++;
    scmd->outR[scmd->outRcount] = NULL;
}

void insertAppendRedir(subCommand * scmd ,char * appendRedir)
{
     if(scmd->appRcount >= 63)
    {
        printf("To many append redirections. Are they necesary?\n");
        exit(1);
    }
    
    scmd->appR[scmd->appRcount] = appendRedir;
    scmd->appRcount++;
    scmd->appR[scmd->appRcount] = NULL;
}
