#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

struct Op{
    int n1, n2;
};

void * sum(void * arg){
    struct Op *Oper = (struct Op*) arg;
    printf("%d + %d = %d\n", Oper->n1, Oper->n2, Oper->n1 + Oper->n2);
    return NULL;
}

void * difference(void * arg){
    struct Op *Oper = (struct Op*) arg;
    printf("%d - %d = %d\n", Oper->n1, Oper->n2, Oper->n1 - Oper->n2);
    return NULL;
}

void * multiplication(void * arg){
    struct Op *Oper = (struct Op*) arg;
    printf("%d * %d = %d\n",  Oper->n1, Oper->n2, Oper->n1 * Oper->n2);
    return NULL;
}

void * division(void * arg){
    struct Op *Oper = (struct Op*) arg;
    if(Oper->n2 != 0)
        printf("%d / %d = %f\n",  Oper->n1, Oper->n2, (float)Oper->n1 / (float)Oper->n2);
    else printf("Operation / invalid\n");
    return NULL;
}

int main(){
    pthread_t pid1, pid2, pid3, pid4;
    struct Op Oper;
    printf("x y ?");
    scanf("%d", &Oper.n1);
    scanf("%d", &Oper.n2);
    pthread_create(&pid1, NULL, sum, &Oper);
    pthread_create(&pid2, NULL, difference, &Oper);
    pthread_create(&pid3, NULL, multiplication, &Oper);
    pthread_create(&pid4, NULL, division, &Oper);
    pthread_exit(0);
}