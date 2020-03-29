#ifndef EXECUTE
#define EXECUTE
#include "shell_structures.h"

int Execute(Command * cmd);

int GetInfd(subCommand * scmd);

int GetOutfd(subCommand * scmd);

int RunCMD(subCommand * scmd, int fdin, int fdout);

#endif