#ifndef EXECUTE
#define EXECUTE
#include "shell_structures.h"

#define JOBS "jobs"
#define BG "bg"
#define FG "fg"
#define CD "cd"
#define PWD "pwd"
#define AGAIN "again"
#define HISTORY "history"
#define EXIT "exit"

 #define BGSIZE 1000000

 //Process background[BGSIZE];
enum state {Stopped, Running};
//const char * States[] = {"Stopped", "Running"};


int ppid;
int sigintcount;
int wpid;

typedef struct Process
{
    pid_t ppid;
    pid_t pid;
    char * state;
}Process;


int CreateProcess(pid_t pid, pid_t ppid, int state);

//Ejecuta una linea de comandos
int Execute(Command * cmd);

//Devuelve el file descriptor del que va a leer un subproceso
int GetInfd(subCommand * scmd);

//Devuelve el file descriptor en el que va a ecribirr un subproceso
int GetOutfd(subCommand * scmd);

//ejecuta un subproceso
int RunCMD(subCommand * scmd, int fdin, int fdout);

void SIGINTHandler(int in);

void SIGTSTPHandler(int in);

void SIGCHLDHandler(int in);

#endif