// PROGRAMA p01b1.c
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define STDERR 2
#define NUMITER 10000

void *thrfunc(void *arg)
{
    fprintf(stderr, "Starting thread %s\n", (char *)arg);
    for (int i = 1; i <= NUMITER; i++)
        write(STDERR, arg, 1);
    return NULL;
}

int main(int argc, char *argv[])
{
     if(argc != 3)
    {
        printf("Usage: %s <char> <char>", argv[0]);
        return 1;
    }

    pthread_t ta, tb;
    pthread_create(&ta, NULL, thrfunc, argv[1]);
    pthread_create(&tb, NULL, thrfunc, argv[2]);
    pthread_join(ta, NULL);
    pthread_join(tb, NULL);
    return 0;
}