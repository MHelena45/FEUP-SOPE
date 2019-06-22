#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#define READ 0
#define WRITE 1
#define BUFFER_SIZE 512

int main()
{
    int fd[2], written;
    char buf[BUFFER_SIZE]
    pid_t pid;

    if (pipe(fd) < 0)
        perror("Error creating pipe");

    pid = fork();

    if (pid < 0)
    {
        perror("ERROR PIPE");
        return 1;
    }

    if (pid > 0) //father
    {
        close(fd[WRITE]);
        dup2(fd[READ], STDIN_FILENO);
        execlp("sort", "sort");
        printf("Couldn't execute sort!\n");
        exit(1); 
        
    }
    else
    { //SON
        close(fd[READ]);
        dup2(fd[WRITE], STDOUT_FILENO);
        execlp("cat", "cat", argv[1]);
        printf("Couldn't execute cat!\n");
        close(fd[WRITE]);   
        exit(1);                 
    }
    return 0;
}