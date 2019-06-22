// PROGRAMA p01b.c
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define STDERR 2
#define NUMITER 10000

void *thrfunc(void *arg)
{
    char buffer[8];
    sprintf(buffer, "%d", *(int *) arg);
    fprintf(stderr, "Starting thread %s\n", (char *)arg);
    for (int i = 1; i <= NUMITER; i++)
        write(STDERR, buffer, 1);
    return NULL;
}
int main(int argc, char *argv[])
{
     if(argc != 3)
    {
        printf("Usage: %s <int> <int>", argv[0]);
        return 1;
    }
    int n1 = atoi(argv[1]);
    int n2 = atoi(argv[2]);
    pthread_t ta, tb;
    pthread_create(&ta, NULL, thrfunc, &n1);
    pthread_create(&tb, NULL, thrfunc, &n2);
    pthread_join(ta, NULL);
    pthread_join(tb, NULL);
    return 0;
}