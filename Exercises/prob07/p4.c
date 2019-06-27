// PROGRAMA p04.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>

#define MAXELEMS 10000000 // nr. max de posicoes
#define MAXTHREADS 100    // nr. max de threads

#define min(a, b) (a) < (b) ? (a) : (b)

#define SEM_NAME "/sem0"

int npos;
int * buf, pos = 0, val = 0;             // variaveis partilhadas

void *fill(void *nr)
{
    while (1)
    {
        sem_wait(&sem);
        if (pos >= npos)
        {
            sem_post(&sem);
            return NULL;
        }
        buf[pos] = val;
        pos++;
        val++;
        sem_post(&sem);
        *(int *)nr += 1;
    }
}

void *verify(void *arg)
{
    int k;
    for (k = 0; k < npos; k++)
        if (buf[k] != k) // detecta valores errados
            printf("ERROR: buf[%d] = %d\n", k, buf[k]);
    return NULL;
}

int main(int argc, char *argv[])
{
    int k, nthr, count[MAXTHREADS];   // array para contagens
    pthread_t tidf[MAXTHREADS], tidv; // tids dos threads
    int total;
    sem_t* sem;

    if (argc != 3)
    {
        printf("Usage: %s <nr_pos> <nr_thrs>\n", argv[0]);
        return 1;
    }
    int shm = shm_open("/shmp0", O_CREAT | O_RDWR, 0600);

    if(shm < 0){
        perror("Couldn't open shared memory");
        exit(1);
    }

    if(ftruncate(shm, sizeof(int)* MAXELEMS) < 0){
        perror("Couldn't allocate memory");
        exit(2);
    }
    
    npos = min(atoi(argv[1]), MAXELEMS);   //no. efectivo de posicoes
    nthr = min(atoi(argv[2]), MAXTHREADS); //no. efectivo de threads
    for (k = 0; k < nthr; k++)
    { // criacao das threads 'fill'
        count[k] = 0;
        pthread_create(&tidf[k], NULL, fill, &count[k]);
    }
    total = 0;
    for (k = 0; k < nthr; k++)
    { //espera threads 'fill'
        pthread_join(tidf[k], NULL);
        printf("count[%d] = %d\n", k, count[k]);
        total += count[k];
    }
    printf("total count = %d\n", total); // mostra total
    pthread_create(&tidv, NULL, verify, NULL);
    pthread_join(tidv, NULL); // espera thread 'verify'


    return 0;
}