/* COPIA FICHEIRO */
/* USO: copy source destination */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#define BUFFER_SIZE 512
int main(int argc, char *argv[])
{
    FILE *infile, *outfile;
    unsigned char buffer[BUFFER_SIZE];

    if (argc != 3)
    {
        printf("Usage: %s <source> <destination>\n", argv[0]);
        return 1;
    }
    infile = fopen(argv[1], "r");
    if (infile == NULL)
    {
        perror(argv[1]);
        return 2;
    }
    outfile = fopen(argv[2], "w");
    if (outfile == NULL)
    {
        perror(argv[2]);
        fclose(infile);
        return 3;
    }
    while ((fgets(buffer, BUFFER_SIZE, infile)) != NULL){
        fputs(buffer, outfile);
    }
    fclose(infile);
    fclose(outfile);
    return 0;
}