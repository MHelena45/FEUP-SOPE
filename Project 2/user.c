/* Cliente  */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <string.h>
#include "banking_aux.h"

    /*
        TODO:
        1 - Create user FIFO
        
        2 - Send command to server FIFO

        3 - Log command and response to ulog.txt

        4 - Terminate when server responds in user FIFO or after FIFO_TIMEOUT_SECS

        5 - Remove FIFO
    */

#define FIFO_TIMEOUT_SECS 30


int main(int argc, char *argv[]){

    if (argc < 6){
        printf("user [Account ID] [Account Password] [Delay in ms] [Operation code] [Argument list]\n");
        exit(-1);
    }

    int user_id = atoi(argv[1]);
    char* user_password = argv[2];
    int delay = atoi(argv[3]);
    int opcode = atoi(argv[4]);
    char* arguments = argv[5]; 

    //User FIFO creation
    char *fifo_name = (char*)malloc(17);
    sprintf(fifo_name, "/tmp/secure_%d", getpid());
    create_fifo(fifo_name);

    //TODO: Write command to server FIFO, wait for server response

    //TODO: Log command and response

    //User FIFO removal
    remove_fifo(fifo_name);

exit(0);
} 
