#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define BUFFER_SIZE 512

struct student
{ 
    unsigned char *name;
    int grade;    
};


int main(int argc, char *argv[]){

    int fd;
    size_t nr, nw;
    unsigned char buffer[BUFFER_SIZE];

    if(argc != 2){
        printf("Usage %s <destinany> \n ", argv[0]);
        return 1;
    }

    fd = open(argv[1], O_WRONLY | O_CREAT | O_EXCL );
    if(fd == -1)
    {
        perror(argv[1]);
        return 2;
    }

    struct student students;
    students.name = malloc(50);
 
    printf("What's the name of the student ?\n");
    while((nr = read(STDIN_FILENO, buffer, BUFFER_SIZE))){
        if(nr <= 1)
            break;
        students.name = buffer; //copy to the struct the name of the student
        printf("What's the grade of the student ?\n");
        if(read(STDIN_FILENO, buffer, BUFFER_SIZE)){
            students.grade = atoi(buffer); //copy to the struct the grade of the student
            if((nw = write(fd, students.name, strlen(students.name))) <= 0 ){
                perror(argv[1]);
                close(fd);
                return 3;
            }
            if((nw = write(fd, students.grade, strlen(students.grade))) <= 0 ){
                perror(argv[1]);
                close(fd);
                return 4;
            }
        }
        printf("What's the grade of the student ?\n");
    }

    close(fd);
    return 0;
     
}