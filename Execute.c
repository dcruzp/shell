#include "shell_structures.h"
#include "parser.h"
#include "Execute.h"
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int Execute(Command * cmd)
{
   int fdpipe[2], fdin, fdout, rcmd, status,
   backupSTDIN = dup(STDIN_FILENO), backupSTDOUT = dup(STDOUT_FILENO);
   pid_t cpid;

   fdin = GetInfd(&cmd->commands[0]);
   if(fdin < 0)
   {
       return -1;
   }

    for (int i = 0; i < cmd->subCommandCount; i++)
    {
        

        fdout = GetOutfd(&cmd->commands[i]);
        if(fdout < 0)
        {
            return -1;
        }
       

        if(i < cmd->subCommandCount - 1)
        {
             int pi = pipe(fdpipe);
            if(pi < 0)
            {
                perror("pipe");
                return -1;
            }
            if(fdout == STDOUT_FILENO)
            {
                fdout = fdpipe[1];            
            }
            else
            {
                close(fdpipe[1]);
            }
        }

        cpid = fork();
        
        if(cpid == 0)
        {
            rcmd = RunCMD(&cmd->commands[i], fdin, fdout);
            if (rcmd < 0)
            {
                return -1;
            }
        }

        if(i < cmd->subCommandCount - 1)
        {
            fdin = GetInfd(&cmd->commands[i+1]);
        }

        if(fdin == STDIN_FILENO)
        {
            fdin = dup(fdpipe[0]);
        }
        else
        {
            close(fdpipe[0]);
        }
        
    }

    if(!cmd->_background)
    {
        int waitResult = waitpid(cpid, &status, 0);
        if(waitResult < 0)
        {
            perror("waitpid");
            return -1;
        }
    }
    
    return 0;
}

int GetInfd(subCommand * scmd)
{
    int outFD = STDIN_FILENO;
    if(scmd->inRcount > 0)
    {
        for (int i = 0; i < scmd->inRcount; i++)
        {
            outFD = open(scmd->inR[i], O_RDONLY);
            if(outFD < 0)
            {
                perror("Not such a file or directory");
                return -1;
            }
        }
    }
    
    return outFD;
}

int GetOutfd(subCommand * scmd)
{
    int outFD = STDOUT_FILENO;
    if(scmd->appRcount > 0)
    {
        for (int i = 0; i < scmd->appRcount; i++)
        {
            outFD = open(scmd->appR[i], O_CREAT | O_APPEND | O_WRONLY);
            if(outFD < 0)
            {
                perror("Trying to open for Append");
                return -1;
            }
        }
    }
    if(scmd->outRcount > 0)
    {
        for (int i = 0; i < scmd->outRcount; i++)
        {
            outFD = open(scmd->outR[i], O_CREAT | O_WRONLY | O_TRUNC);
            if(outFD < 0)
            {
                perror("Trying to open for Writting");
                return -1;
            }
        }
    }

    return outFD;
}

int RunCMD(subCommand * scmd, int fdin, int fdout)
{
    if(fdin != STDIN_FILENO)
    {
        dup2(fdin, STDIN_FILENO);
        close(fdin);
    }
    if(fdout != STDOUT_FILENO)
    {
        dup2(fdout, STDOUT_FILENO);
        close(fdin);
    }

    execvp(scmd->cmd, scmd->args);
    perror("Problem execute a program");

    return -1;
}