// PROGRAMA p8c.c
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char *argv[], char *envp[])
{
    pid_t pid;
    char *arg[3];
    arg[0] = "-laR";
    arg[1] = argv[1];
    arg[2] = NULL;
    if (argc != 2)
    {
        printf("usage: %s dirname\n", argv[0]);
        exit(1);
    }
    pid = fork();
    if (pid > 0)
        printf("My child is going to execute command \"ls -laR %s\"\n", argv[1]);
    else if (pid == 0)
    {
        execvp("-ls", arg );
        printf("Command not executed !\n");
        exit(1);
    }
    exit(0);
}
