// PROGRAMA p02.c
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h> 

#define STDERR 2

int limit = 50000;
void *thrfunc(void *arg)
{
    void *ret;
    int written = 0;
    fprintf(stderr, "Starting thread %s\n", (char *)arg);
    while(limit > 0){
        limit--;
        write(STDERR, arg, 1);
        written++;
    }     
    printf("\nWhere written %d caracters\n", written); 
    ret = malloc(sizeof(int));
    *(int*) ret = written;  
    return ret;
}
int main()
{
    void *writtena, *writtenb;
    pthread_t ta, tb;
    pthread_create(&ta, NULL, thrfunc, "1");
    pthread_create(&tb, NULL, thrfunc, "2");
    pthread_join(ta, &writtena);
    pthread_join(tb, &writtenb);
    printf("Total of: %d\n", *(int * )writtena + *(int * )writtenb);
    free(writtena);
    free(writtena);
    return 0;
}