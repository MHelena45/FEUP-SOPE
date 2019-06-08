#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#define MAX 50000
int main(void)
{
    int i;
    pid_t pid;
    char str[10];
    pid = fork();
    switch (pid)
    {
    case -1:
        perror("fork");
        break;
    case 0: //filho
        sleep(1);
        printf("world!");        
        break;
    default: //pai
        printf("Hello ");
        break;
    }
    return 0;
}
