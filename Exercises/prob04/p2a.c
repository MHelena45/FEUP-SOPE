// PROGRAMA p02a.c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void sigint_handler(int signo)
{
    if (signo == SIGINT)
    {
        printf("Entering SIGINT handler ...\n");
        sleep(10);
        printf("Exiting SIGINT handler ...\n");
    }
    else{
        printf("Entering SIGTERM handler ...\n");
        sleep(1);
        printf("Exiting SIGTERM handler ...\n");
    }

}
int main(void)
{
    struct sigaction action;
    action.sa_handler = sigint_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    if (sigaction(SIGINT, &action, NULL) < 0 || sigaction(SIGTERM, &action, NULL) < 0)
    {
        fprintf(stderr, "Unable to install one handler\n");
        exit(1);
    }
    printf("Try me with CTRL-C ...\n");
    while (1)
        pause();
    exit(0);
}