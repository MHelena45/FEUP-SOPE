// PROGRAMA p5b.c
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    int fd;
    char *text1 = "CCCCC";
    char *text2 = "DDDDD";
    fd = open("f1.txt", O_WRONLY | O_SYNC, 0600);
    write(fd, text1, 5);
    write(fd, text2, 5);
    close(fd);
    return 0;
}

/* The flags " O_WRONLY | O_SYNC"
O_WRONLY - abertura só para escrita 
O_SYNC - só retorna depois de os dados terem sido fisicamente escritos

Se o ficheiro já existir, o conteudo do ficheiro será CCCCCDDDDD
Se o ficheiro não existir, um não será criado
*/