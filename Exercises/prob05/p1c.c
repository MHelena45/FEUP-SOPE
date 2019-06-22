#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

#define READ 0
#define WRITE 1

int main()
{
    struct Numbers
    {
        char n1[3], n2[3];
    } num;

    int fd[2];
    pid_t pid;

    if (pipe(fd) < 0)
        perror("Error creating pipe");

    pid = fork();

    if (pid > 0) //father
    {
        printf("PARENT:\n");
        printf(" x y ? ");
        scanf("%c %c", num.n1, num.n2);
        close(fd[READ]);
        write(fd[WRITE], &num, sizeof(num));
        close(fd[WRITE]);
    }
    else
    { //SON
        close(fd[WRITE]);
        //process only passes were when "write(fd[WRITE], a, 2*sizeof(int));" is done
        read(fd[READ], &num, sizeof(num));
        printf("SON:\n");
        printf("x + y = %d\n", atoi(num.n1) + atoi(num.n2));
        printf("x - y = %d\n", atoi(num.n1) - atoi(num.n2));
        printf("x * y = %f\n", (float)atoi(num.n1) * (float)atoi(num.n2));
        if (num.n2 != 0)
            printf("x / y = %f\n", (float)atoi(num.n1) / (float)atoi(num.n2));
        else
            printf("Can't divide the numbers\n");
        close(fd[READ]);
    }
    return 0;
}