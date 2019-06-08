// PROGRAMA p1.c
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int global = 1;
int main(void)
{
    int local = 2;
    if (fork() > 0)
    {
        //processo-pai
        printf("PID = %d; PPID = %d\n", getpid(), getppid());
        global++;
        local--;
    }
    else
    {
        //processo-filho
        printf("PID = %d; PPID = %d\n", getpid(), getppid());
        global--;
        local++;
    }
    printf("PID = %d - global = %d ; local = %d\n", getpid(), global, local);
    return 0;
}