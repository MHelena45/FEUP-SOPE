/* Cliente  */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <string.h>
#include "banking_aux.h"
#include "types.h"
#include "sope.h"
#include "constants.h"

    /*
        TODO:
        1 - Create user FIFO
        
        2 - Send command to server FIFO

        3 - Log command and response to ulog.txt

        4 - Terminate when server responds in user FIFO or after FIFO_TIMEOUT_SECS

        5 - Remove FIFO
    */


int main(int argc, char *argv[]){

    if (argc < 6){
        printf("user [Account ID] [Account Password] [Delay in ms] [Operation code] [Argument list]\n");
        exit(-1);
    }

    int user_id = atoi(argv[1]);
    char* user_password = argv[2];
    if (!is_valid_password(user_password)){
        printf ("Password needs to have between %d and %d characters\n", MIN_PASSWORD_LEN, MAX_PASSWORD_LEN);
        exit(-1);
    }
    int delay = atoi(argv[3]);
    op_type_t opcode = atoi(argv[4]);
    char* arguments = argv[5]; 

    //User FIFO creation
    char *fifo_path = (char*)malloc(USER_FIFO_PATH_LEN);
    sprintf(fifo_path, "%s%0*d",USER_FIFO_PATH_PREFIX, WIDTH_ID,getpid());
    printf(fifo_path);
    create_fifo(fifo_path);

    //TODO: Write command to server FIFO, wait for server response

    //TODO: Log command and response

    //User FIFO removal
    remove_fifo(fifo_path);

exit(0);
} 
