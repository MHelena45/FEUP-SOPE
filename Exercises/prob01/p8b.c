// PROGRAMA p8b.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    time_t t;
    int n;
    struct tms start_time, end_time;
    double ticks_seg = sysconf(_SC_CLK_TCK);

    /* Initializes random number generator */
    srand((unsigned) time(&t));    

    times(&start_time);

    if(argc != 3){
        printf("Usage: %s <number> <number> \n ", argv[0]);
        return -1;
    }      

    while((n = rand() % atoi(argv[1])) != atoi(argv[2])){
        printf("%d ", n );
    }

    times(&end_time);

    printf("\nTempo real: %f \n", ((end_time.tms_utime) - (start_time.tms_utime))/ticks_seg);
    printf("Tempo de CPU no modo user: %f \n", ((end_time.tms_cutime) - (start_time.tms_utime))/ticks_seg);
    printf("Tempo de CPU no modo system : %f \n", ((end_time.tms_cstime) - (start_time.tms_cstime))/ticks_seg);
    
   return 0;
}
