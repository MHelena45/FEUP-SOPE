// PROGRAMA p01b.c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
void sigint_handler(int signo)
{
    printf("In SIGINT handler ...\n");
}
int main(void)
{
    int duraction = 30;
    if (signal(SIGINT, sigint_handler) < 0)
    {
        fprintf(stderr, "Unable to install SIGINT handler\n");
        exit(1);
    }
    printf("Sleeping for 30 seconds ...\n");
    while(sleep(duraction));
    printf("Waking up ...\n");
    exit(0);
}