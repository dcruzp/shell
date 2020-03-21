#include "shell_structures.h"


Command * initCommand(int subCmdCount, int background, char * comment, char * _stdin, char * _stdout, char * _stderr)
{
    Command * cmd = (Command*)malloc(sizeof(Command));
    cmd->currentSubcmd = 0;
    cmd->subCommandCount = subCmdCount;
    cmd->_background = background;
    cmd->comment = comment;
    cmd->_stdin = _stdin;
    cmd->_stdout = _stdout;
    cmd->_stderr = _stderr;
    cmd->commands = (subCommand*)malloc(sizeof(subCommand) * subCmdCount);
    printf("count: %d\n", subCmdCount);
    printf("cmd->subc: %d\n", cmd->subCommandCount);
    return cmd;
}

void CommandDestructor(Command * cmd, char * arguments)
{
    free(cmd->commands);
    free(cmd);
}

subCommand * initSubCommand(char * cmd, char * Arguments)
{
    subCommand * subcmd = (subCommand*)malloc(sizeof(subCommand));
    subcmd->cmd = cmd;
    subcmd->arguments = Arguments;

    return subcmd;
}

void SubCommandestructor(subCommand * subcmd)
{
    free(subcmd);
}


void insertSubcommand(Command * cmd, subCommand * subcmd)
{
    if(cmd->subCommandCount == cmd->currentSubcmd)
    {
        printf("WARNING: Commands are full.\n");
    }
    int index = cmd->currentSubcmd;
    cmd->commands[index] = *subcmd;
    cmd->currentSubcmd++;
}

void PrintCMD(Command * cmd)
{
    int count = cmd->subCommandCount;
    printf("Ampersand: %d\nin: %s\nout: %s\n", cmd->_background, cmd->_stdin, cmd->_stdout);
    for (int i = 0; i < count; i++)
    {
        subCommand aux = cmd->commands[i];
        char * _cmd = aux.cmd;
        char * args = aux.arguments;
        printf("\nComand #%d:\n-cmd: %s\n-args: %s\n", i, _cmd, args);
    }
    
}
