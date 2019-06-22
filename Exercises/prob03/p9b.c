// PROGRAMA p9b.c
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char *argv[], char *envp[])
{
    pid_t pid;
    int status;
    if (argc != 2)
    {
        printf("usage: %s dirname\n", argv[0]);
        exit(1);
    }
    pid = fork();
    if (pid > 0){
        //processo-pai
        printf("My child is going to execute command \"ls -laR %s\"\n", argv[1]);
        wait(&status); //processo-pai espera pela terminação do processo-filho
         printf("My child returned with exit code %d\n", status);
        if(WIFEXITED(status))
            printf("Process son end normally");
        else
        {
            printf("Process son end anormally");
        }
        

    }        
    else if (pid == 0)
    {
        execlp("ls", "ls","-laR", NULL );
        printf("Command not executed !\n");
        exit(1);
    }
    exit(0);
}