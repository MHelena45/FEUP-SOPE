#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

void * sum(void * arg){
    int * arguments = (int *) arg;
    printf("%d + %d = %d\n", arguments[0], arguments[1],arguments[0] + arguments[1]);
    return NULL;
}

void * difference(void * arg){
    int * arguments = (int *) arg;
    printf("%d - %d = %d\n", arguments[0], arguments[1],arguments[0] - arguments[1]);
    return NULL;
}

void * multiplication(void * arg){
    int * arguments = (int *) arg;
    printf("%d * %d = %d\n", arguments[0], arguments[1],arguments[0] * arguments[1]);
    return NULL;
}

void * division(void * arg){
    int * arguments = (int *) arg;
    if(arguments[1] != 0)
        printf("%d / %d = %f\n", arguments[0], arguments[1], (float) arguments[0] / (float) arguments[1]);
    else printf("Operation / invalid\n");
    return NULL;
}

int main(){
    int arguments[2];
    pthread_t pid1, pid2, pid3, pid4;
    printf("x y ?");
    scanf("%d", &arguments[0]);
    scanf("%d", &arguments[1]);
    pthread_create(&pid1, NULL, sum, arguments);
    pthread_create(&pid2, NULL, difference, arguments);
    pthread_create(&pid3, NULL, multiplication, arguments);
    pthread_create(&pid4, NULL, division, arguments);
    pthread_exit(0);
}