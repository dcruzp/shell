#include "shell_structures.h"
#include "parser.h"
#include "Execute.h"
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

int Contin = 0;
int Execute(Command *cmd)
{
    int fdpipe[2], fdin, fdout, rcmd, status,
    backupSTDIN = dup(STDIN_FILENO), backupSTDOUT = dup(STDOUT_FILENO);
    pid_t cpid;
    ppid = getpid();
    sigintcount = 0;
    signal(SIGINT, SIGINTHandler);
    signal(SIGCHLD, SIGCHLDHandler);
    signal(SIGTSTP, SIGTSTPHandler);
    
    fdin = GetInfd(&cmd->commands[0]);
    if (fdin < 0)
    {
        return -1;
    }

    for (int i = 0; i < cmd->subCommandCount; i++)
    {

        fdout = GetOutfd(&cmd->commands[i]);
        if (fdout < 0)
        {
            return -1;
        }

        if (i < cmd->subCommandCount - 1)
        {
            int pi = pipe(fdpipe);
            if (pi < 0)
            {
                perror("pipe");
                return -1;
            }
            if (fdout == STDOUT_FILENO)
            {
                fdout = fdpipe[1];
            }
            else
            {
                close(fdpipe[1]);
            }
        }

        if(strcmp(cmd->commands[i].cmd,"exit") == 0)
        {
            return 2;
        }

        cpid = fork();
        if (cpid == 0)
        {
            signal(SIGINT, SIG_IGN);
            signal(SIGTSTP, SIG_DFL);
            rcmd = RunCMD(&cmd->commands[i], fdin, fdout);
            if (rcmd < 0)
            {
                return -1;
            }
        }

        if (fdin != GetInfd(&cmd->commands[i]))
        {
            close(fdin);
        }

        if (fdout != GetOutfd(&cmd->commands[i]))
        {
            close(fdout);
        }

        if (i < cmd->subCommandCount - 1)
        {
            fdin = GetInfd(&cmd->commands[i + 1]);
        }

        if (fdin == STDIN_FILENO)
        {
            fdin = fdpipe[0];
        }
        else
        {
            close(fdpipe[0]);
        }
    }

    if (cmd->_background == 0)
    {
        wpid = cpid;

        while(1)
        {
            int status;
            int out;
            out = waitpid(cpid, &status, WUNTRACED);
            if(WIFEXITED(status))
            {
                break;
            }
            if(WIFSTOPPED(status))
            {
                break;
            }
        }
        wpid = -1;
    }
    else
    {
        printf("[%d]\n", cpid);
    }

    return 0;
}

int GetInfd(subCommand *scmd)
{
    int outFD = STDIN_FILENO;
    if (scmd->inRcount > 0)
    {
        for (int i = 0; i < scmd->inRcount; i++)
        {
            outFD = open(scmd->inR[i], O_RDONLY);
            if (outFD < 0)
            {
                perror("Not such a file or directory");
                return -1;
            }
        }
    }

    return outFD;
}

int GetOutfd(subCommand *scmd)
{
    int outFD = STDOUT_FILENO;
    if (scmd->appRcount > 0)
    {
        for (int i = 0; i < scmd->appRcount; i++)
        {
            outFD = open(scmd->appR[i], O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
            if (outFD < 0)
            {
                perror("Trying to open for Append");
                return -1;
            }
        }
    }
    if (scmd->outRcount > 0)
    {
        for (int i = 0; i < scmd->outRcount; i++)
        {
            outFD = open(scmd->outR[i], O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
            if (outFD < 0)
            {
                perror("Trying to open for Writting");
                return -1;
            }
        }
    }

    return outFD;
}

int RunCMD(subCommand *scmd, int fdin, int fdout)
{
    if (fdin != STDIN_FILENO)
    {
        dup2(fdin, STDIN_FILENO);
        close(fdin);
    }
    if (fdout != STDOUT_FILENO)
    {
        dup2(fdout, STDOUT_FILENO);
        close(fdin);
    }
    execvp(scmd->cmd, scmd->args);
    perror("Problem execute a program");

    return -1;
}

int ExecuteBuitins(subCommand * scmd, int infd, int outfd)
{
    if(strcmp(scmd->cmd, JOBS) == 0)
    {
        //jobs execution here
    }
    else if (strcmp(scmd->cmd, BG) == 0)
    {
        /* code */
    }
    else if (strcmp(scmd->cmd, FG) == 0)
    {
        /* code */
    }
    else if (strcmp(scmd->cmd, CD) == 0)
    {
        /* code */
    }
    else if (strcmp(scmd->cmd, PWD) == 0)
    {
        /* code */
    }
    else if (strcmp(scmd->cmd, AGAIN) == 0)
    {
        /* code */
    }
    else if (strcmp(scmd->cmd, HISTORY) == 0)
    {
        /* code */
    }
    else if (strcmp(scmd->cmd, EXIT) == 0)
    {
        /* code */
    }
    else
    {
        return 0;
    }

}

void SIGINTHandler(int in)
{
        if(wpid < 0)
        { 
            write(STDOUT_FILENO, "\nPrompt $ ", 11);
            return;
        }
        if(sigintcount < 1)
        { 
            write(STDOUT_FILENO, "Press ctrl+c again to teminate\n", 32);
            sigintcount++;
        }
        else
        { 
            sigintcount = 0; 
            kill(wpid, SIGKILL);
            wpid = -1;
        }
}

void SIGTSTPHandler(int in)
{
    if(wpid == -1)
    {
        write(STDOUT_FILENO, "\nPrompt $ ", 11);
    }
}

void SIGCHLDHandler(int in)
{
    int status;
    int auxpid = waitpid(wpid, &status, WNOHANG);
    if(!WIFEXITED(status) && WIFSTOPPED(status))
    {
        kill(wpid, SIGSTOP);
    }

    wpid = -1;
}