// PROGRAMA p6g.c
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define BUF_LENGTH 256

int main(int argc, char * argv[])
{
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

//if we execute without name of the files errno is 14