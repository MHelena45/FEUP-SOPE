// PROGRAMA p8a.c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    time_t t;
    int n;
    
    if(argc != 3){
        printf("Usage: %s <number> <number> \n ", argv[0]);
        return -1;
    }
    
    /* Intializes random number generator */
    srand((unsigned) time(&t));

    while((n = rand() % atoi(argv[1])) != atoi(argv[2])){
        printf("%d \n", n );
    }
    
   return 0;
}