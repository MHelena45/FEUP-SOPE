// PROGRAMA p8e.c
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char *argv[], char *envp[])
{
    pid_t pid;
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
        char *arg[4];
        arg[0] = "/bin/ls";
        arg[1] = "-laR";
        arg[2] = argv[1];
        arg[3] = NULL;
        execve("/bin/ls", arg, envp);
        printf("Command not executed !\n");
        exit(1);
    }
    exit(0);
}