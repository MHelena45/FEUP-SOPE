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
#include <pthread.h>

#include "banking_aux.h"
#include "types.h"
#include "sope.h"
#include "constants.h"

int main(int argc, char *argv[]){

    if (argc < 3){
        printf("server [Number of bank offices] [Administrator password]\n");
        exit(EXIT_FAILURE);
    }
    int threads_number = atoi(argv[1]);
    if (threads_number > MAX_BANK_OFFICES){
        printf("The maximum number of bank offices is %d\n", MAX_BANK_OFFICES);
        exit(EXIT_FAILURE);
    }
    char *admin_password = argv[2];
    if (!is_valid_password(admin_password)){
        printf ("Password needs to have between %d and %d characters\n", MIN_PASSWORD_LEN, MAX_PASSWORD_LEN);
        exit(EXIT_FAILURE);
    }
    /**
     * Initiate threads and bank accounts arrays
     */
    bank_account_t accounts [MAX_BANK_ACCOUNTS];
    pthread_t threads[threads_number];
    /**
     * Create admin account
     */
    accounts[ADMIN_ACCOUNT_ID].account_id = ADMIN_ACCOUNT_ID;
    accounts[ADMIN_ACCOUNT_ID].balance = 0;
    //TODO: Hash password using a pipe to sha256sum
    //accounts[ADMIN_ACCOUNT_ID].hash
    //accounts[ADMIN_ACCOUNT_ID].salt

    /**
     * Server FIFO creation
     */
    create_fifo(SERVER_FIFO_PATH);

    /**
     * TODO: Create server threads
     * TODO: Handle requests to server fifo
     * TODO: Log thread sync actions to SERVER_LOGFILE
     * TODO: Log requests to SERVER_LOGFILE
     * TODO: Reply to the correct user fifo
     * TODO: Log reply to SERVER_LOGFILE
     * TODO: Handle all pending requests before exiting when receiving shutdown request
     */

    //Server FIFO removal
    remove_fifo(SERVER_FIFO_PATH);

 exit(EXIT_SUCCESS);
} 
