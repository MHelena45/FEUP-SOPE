#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#define READ 0
#define WRITE 1

int main(int argc, char *argv[])
{
    int fd1[2], fd2[2];
    if (pipe(fd2) < 0)
        perror("Error creating pipe");

    if (fork() > 0)
    {
        close(fd2[WRITE]);
        if (pipe(fd1) < 0)
            perror("Error creating pipe");
        if (fork() > 0)
        {
            close(fd1[WRITE]);
            close(fd2[READ]);
            dup2(fd1[READ], STDIN_FILENO);
            execlp("sort", "sort", NULL); //sort
            printf("Failed to execute sort!\n");
            exit(1);
        }
        else
        {
            close(fd1[READ]);
            dup2(fd2[READ], STDIN_FILENO);
            dup2(fd1[WRITE], STDOUT_FILENO);
            execlp("grep", "grep", argv[1], NULL);//grep arg
            printf("Failed to execute grep!\n");
            exit(1);
        }
    }
    else
    {
        close(fd2[READ]);
        dup2(fd2[WRITE], STDOUT_FILENO);
        execlp("ls", "ls", "-laR", argv[2], NULL); //ls dir –laR
        printf("Failed to execute ls!\n");
        exit(1);
    }
    return 0;
}