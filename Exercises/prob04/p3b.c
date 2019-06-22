// PROGRAMA p3a.c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int v;

void sigint_handler(int signo)
{
    if (signo == SIGUSR1)
    {
       // printf("SIGUSR1 handler ...\n");
        printf("V: %d \n", v++);
    }
    else
    {
        //printf("SIGUSR2 handler ...\n");
        printf("V: %d \n", v--);
    }
}
int main(void)
{
    int pid = fork();

    srand(time(NULL));

    if(pid < 0){
        printf("Fork went wrong!");
        exit(1);
    }
    else if(pid == 0){
        struct sigaction action;
        action.sa_handler = sigint_handler;
        sigemptyset(&action.sa_mask);
        action.sa_flags = 0;
        if (sigaction(SIGUSR1, &action, NULL) < 0 || sigaction(SIGUSR2, &action, NULL) < 0)
        {
            fprintf(stderr, "Unable to install one handler\n");
            exit(1);
        }
        while (1)
            pause();
    }  else {
        for (int i = 0; i < 50; i++)
        {
            if (rand() % 2 == 0) {
                kill(pid, SIGUSR2);
            }
            else
            {
                kill(pid, SIGUSR1);
            }
            sleep(1);
        }
        kill(pid, SIGTERM);
    }  

    exit(0);
}