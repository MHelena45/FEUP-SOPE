// PROGRAMA p5b.c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void sigchld_handler(int sig)
{
    printf("SIGCHILD Handler\n");
    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;
}

int main(void)
{
    pid_t pid;
    int i, n, status;

    //subscribing the SIGCHLD handler
    struct sigaction act;
    act.sa_handler = sigchld_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if(sigaction(SIGCHLD, &act, NULL)){
        printf("Error in sigaction\n");
        exit(1);
    }

    for (i = 1; i <= 3; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            printf("CHILD no. %d (PID=%d) working ... \n", i, getpid());
            sleep(i * 7); // child working ...
            printf("CHILD no. %d (PID=%d) exiting ... \n", i, getpid());
            exit(0);
        }
    }

    for (i = 1; i <= 4; i++)
    {
        printf("PARENT: working hard (task no. %d) ...\n", i);
        n = 20;
        while ((n = sleep(n)) != 0)
            ;
        printf("PARENT: end of task no. %d\n", i);
        printf("PARENT: waiting for child no. %d ...\n", i);
        pid = waitpid(-1, &status, WNOHANG);
        if (pid != -1)
            printf("PARENT: child with PID=%d terminated with exit code %d\n", pid, WEXITSTATUS(status));
    }
    exit(0);
}