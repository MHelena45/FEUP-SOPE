// PROGRAMA p6h.c
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define BUF_LENGTH 256

int main(int argc, char * argv[])
{
    if(argc != 3){
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        return -1;
    }
    FILE *src, *dst;
    char buf[BUF_LENGTH];
    if ((src = fopen(argv[1], "r")) == NULL)
    {
        printf("Couldn't open inputfile.txt. The errno is %d\n", errno);
        exit(1); //couldn't open infile.txt
    }
    if ((dst = fopen(argv[2], "w")) == NULL)
    {
        printf("Couldn't open outputfile.txt. The errno is %d\n", errno);
        exit(2); //couldn't open outfile.txt
    }
    while ((fgets(buf, BUF_LENGTH, src)) != NULL)
    {
        fputs(buf, dst);
    }
    fclose(src);
    fclose(dst);
    exit(0); // success 
}