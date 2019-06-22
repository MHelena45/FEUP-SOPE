#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#define READ 0
#define WRITE 1

int main()
{
    int fd[2];
    int fd1[2];
    pid_t pid;

    if (pipe(fd) < 0 || pipe(fd1) < 0)
        perror("Error creating pipe");

    pid = fork();

    if (pid < 0)
    {
        perror("ERROR PIPE");
        return 1;
    }

    if (pid > 0) //father
    {
        int a[2];
        printf("PARENT:\n");
        printf(" x y ? ");
        scanf("%d %d", &a[0], &a[1]);
        close(fd[READ]);
        close(fd1[WRITE]);
        write(fd[WRITE], a, 2 * sizeof(int));
        close(fd[WRITE]);

        int res2[2];
        float res3[2];

        read(fd1[READ], res2, 2 * sizeof(int));
        read(fd1[READ], res3, 2 * sizeof(float));
        printf("PARENT:\n");
        printf("x + y = %d\n", res2[0]);
        printf("x - y = %d\n", res2[1]);
        printf("x * y = %f\n", res3[0]);
        printf("x / y = %f\n", res3[1]);
        close(fd1[WRITE]);
    }
    else
    { //SON
        int b[2], res[2];
        float res1[2];
        close(fd[WRITE]);
        close(fd1[READ]);
        //process only passes were when "write(fd[WRITE], a, 2*sizeof(int));" is done
        read(fd[READ], b, 2 * sizeof(int));
        printf("SON:\n");
        res[0] = b[0] + b[1];
        res[1] = b[0] - b[1];
        res1[0] = (float)b[0] * (float)b[1];
        if (b[1] != 0)
        {
            res1[1] = (float)b[0] / (float)b[1];
        }
        else
        {
            res1[1] = 0;
            printf("Can't divide the numbers\n");
        }
        close(fd[READ]);

        write(fd1[WRITE], res, 2 * sizeof(int));
        write(fd1[WRITE], res1, 2 * sizeof(double));
        close(fd1[WRITE]);
    }
    return 0;
}