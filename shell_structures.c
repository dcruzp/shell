typedef struct subCommand{
    int infd;
    int outfd;
    char cmd[16];
    char args[1024];
}subComand;

typedef struct Command{
    int count;
    char comment[1024];
    subComand commands[64];
}Command;

