// PROGRAMA p04c.c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 20
void *PrintHello(void *threadnum)
{
    void *ret;
    sleep(1);
    printf("\nHello from thread no. %d!", *(int *)threadnum);

    ret = malloc(sizeof(long));
    *(long *)ret = pthread_self();
    return ret;
}
int main()
{
    void *ID;
    pthread_t threads[NUM_THREADS];
    int t;
    int threads_numbers[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
    for (t = 0; t < NUM_THREADS; t++)
    {
        pthread_create(&threads[t], NULL, PrintHello, (void *)&threads_numbers[t]);
        pthread_join(threads[t], &ID);
        printf("Thread id: %ld\n", *(long *)ID);
    }

    pthread_exit(0);
}