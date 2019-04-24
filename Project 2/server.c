/*   servidor      */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include "banking_aux.h"
#include "types.h"
#include "sope.h"
#include "constants.h"

    /*
        TODO:
        1 - Create admin account (bank_account_t?),  password in arguments
        
        2 - Create threads (max 99)
            2.1 - Log thread operations: "I - thread id - OPEN/CLOSE 'POSIX THREAD ID'"
                    (logBankOfficeOpen/Close()?) ex: "I - 00001 - OPEN 140177458939648"

        3 - Create FIFO /tmp/secure_srv
        
        4 - Listen for requests, close when terminated by admin
            4.1 - Can only terminate after handling all pending requests, make FIFO read only
                  till close.
            4.2 - Log requests and responses to slog.txt
        
        5 - Remove FIFO
    */


int main(int argc, char *argv[]){

    if (argc < 3){
        printf("server [Number of threads] [Administrator password]\n");
        exit(-1);
    }
    int threads_number = atoi(argv[1]);
    char *admin_password = argv[2];
    if (!is_valid_password(admin_password)){
        printf ("Password needs to have between %d and %d characters\n", MIN_PASSWORD_LEN, MAX_PASSWORD_LEN);
        exit(-1);
    }


    int server_fifo_fd;

    //TODO: Create admin account
    //TODO: Create server threads
    
    //Server FIFO creation
    create_fifo(SERVER_FIFO_PATH);

    //TODO: Listen and process requests, log requests and responses

    //Server FIFO removal
    remove_fifo(SERVER_FIFO_PATH);

 exit(0);
} 