#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

struct Op
{
    int n1, n2;
};

void *sum(void *arg)
{
    struct Op *Oper = (struct Op *)arg;
    int *res = malloc(sizeof(int));
    *res = Oper->n1 + Oper->n2;
    return res;
}

void *difference(void *arg)
{
    struct Op *Oper = (struct Op *)arg;
    int *res = malloc(sizeof(int));
    *res = Oper->n1 - Oper->n2;
    return res;
}

void *multiplication(void *arg)
{
    struct Op *Oper = (struct Op *)arg;
    int *res = malloc(sizeof(int));
    *res = Oper->n1 * Oper->n2;
    return res;
}

void *division(void *arg)
{
    struct Op *Oper = (struct Op *)arg;
    float *res = malloc(sizeof(float));
    if (Oper->n2 != 0)
        *res = (float)Oper->n1 / (float)Oper->n2;    
    else res = NULL;
    return res;
}

int main()
{
    pthread_t pid1, pid2, pid3, pid4;
    struct Op Oper;
    void *res1, *res2, *res3, *res4;
    printf("x y ?");
    scanf("%d", &Oper.n1);
    scanf("%d", &Oper.n2);
    pthread_create(&pid1, NULL, sum, &Oper);
    pthread_create(&pid2, NULL, difference, &Oper);
    pthread_create(&pid3, NULL, multiplication, &Oper);
    pthread_create(&pid4, NULL, division, &Oper);

    pthread_join(pid1, &res1);
    printf("%d + %d = %d\n", Oper.n1, Oper.n2, *(int *)res1);
    pthread_join(pid2, &res2);
    printf("%d - %d = %d\n", Oper.n1, Oper.n2, *(int *)res2);
    pthread_join(pid3, &res3);
    printf("%d * %d = %d\n", Oper.n1, Oper.n2, *(int *)res3);
    pthread_join(pid4, &res4);
    if ((float *)res4 == NULL)
        printf("Operation / invalid\n");
    else
        printf("%d / %d = %f\n", Oper.n1, Oper.n2, *(float *)res4);
    free(res1);
    free(res2);
    free(res3);
    free(res4);

    pthread_exit(0);
}