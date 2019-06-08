/* 
    programa que lê do teclado os nomes dos alunos de uma disciplina e a respectiva
    classificação, gravando‐os num ficheiro de texto. 
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER_SIZE 512

int main(int argc, char *argv[]){
    int fd, nr, nw;
    unsigned char buffer[BUFFER_SIZE];

    if(argc != 2){
        printf("Usage %s <destination> \n", argv[0]);
        return 1;
    }

    fd = open(argv[1], O_WRONLY | O_CREAT ); //if file exists writes on top of it
    if(fd == -1)
    {
        perror(argv[1]);
        return 3;
    }
    printf("What's the name and grade of the student ?\n");
    while((nr = read(STDIN_FILENO, buffer, BUFFER_SIZE))){
        if (nr <= 1) 
            break;
        if((nw = write(fd, buffer, nr)) <= 0 || nw != nr){
            perror(argv[1]);
            close(fd);
            return 2;
        }
        printf("What's the name and grade of the student ?\n");
    }

    close(fd);
    return 0;
    
}