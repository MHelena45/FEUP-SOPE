#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#define MAX 50000
int main(void)
{
    int i;
    pid_t pid, pid1;
    char str[10];
    pid = fork();
    switch (pid)
    {
        case -1:
            perror("fork");
            break;
        case 0: //filho
            printf("Hello ");
                break;
        default: //pai            
        {
            sleep(2);
            pid1 = fork();
            switch (pid1)
            {
                case -1:
                    perror("fork");
                    break;
                case 0: //filho
                    printf("my ");
                    break;
                default: //pai
                    sleep(1);
                    printf("friends!");
                    break;
            }
            break;
        }     
        


    }
    return 0;
}
