// PROGRAMA p3a.c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

bool goUP = false;

void sigint_handler(int signo)
{
    if (signo == SIGUSR1)
    {
        printf("SIGUSR1 handler ...\n");
        goUP = true;
    }
    else{
        printf("SIGUSR2 handler ...\n");
        goUP = false;
    }

}
int main(void)
{
    int v = 0;
    struct sigaction action;
    action.sa_handler = sigint_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    if (sigaction(SIGUSR1, &action, NULL) < 0 || sigaction(SIGUSR2 , &action, NULL) < 0)
    {
        fprintf(stderr, "Unable to install one handler\n");
        exit(1);
    }
    while (1){
        printf("V: %d \n",v);
        if(goUP)
            v++;
        else v--;
        sleep(1);        
    }
    exit(0);
}
