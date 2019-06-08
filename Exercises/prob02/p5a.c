// PROGRAMA p5a.c
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    int fd;
    char *text1 = "AAAAA";
    char *text2 = "BBBBB";
    fd = open("f1.txt", O_CREAT | O_EXCL | O_TRUNC | O_WRONLY | O_SYNC, 0600);
    write(fd, text1, 5);
    write(fd, text2, 5);
    close(fd);
    return 0;
}
/* The flags " O_CREAT | O_EXCL | O_TRUNC | O_WRONLY | O_SYNC"
O_CREAT - p/ criar o ficheiro se ele não existir; requer mode
O_EXCL - origina erro se o ficheiro existir e O_CREAT estiver activada
O_TRUNC - se o ficheiro existir fica com o comprimento 0
O_WRONLY - abertura só para escrita 
O_SYNC - só retorna depois de os dados terem sido fisicamente escritos

logo executando apenas este programa o conteudo do ficheiro será AAAAABBBBB
*/