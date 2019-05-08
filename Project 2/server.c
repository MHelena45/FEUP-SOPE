/*   servidor      */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <pthread.h>
#include <time.h>

#include "banking_aux.h"
#include "types.h"
#include "sope.h"
#include "constants.h"
#include "general_aux.h"

int main(int argc, char *argv[]){

    /** Verify command **/
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
    
    /** Initiate random seed **/
    srand(time(NULL));

    /** Initiate threads and bank accounts arrays **/
    bank_account_t accounts [MAX_BANK_ACCOUNTS];
    pthread_t threads[threads_number]; //TODO: Create server threads
    
    /** Create admin account **/
    create_bank_account(&accounts[ADMIN_ACCOUNT_ID], admin_password, ADMIN_ACCOUNT_ID, 0);
    int server_log_fd = open(SERVER_LOGFILE, O_CREAT | O_WRONLY | O_APPEND);
    logAccountCreation(server_log_fd, 0, &accounts[ADMIN_ACCOUNT_ID]);
    close(server_log_fd);

    /** Server FIFO creation **/
    create_fifo(SERVER_FIFO_PATH);

    /**
     * TODO: Handle requests to server fifo
     * TODO: Log thread sync actions to SERVER_LOGFILE
     * TODO: Log requests to SERVER_LOGFILE
     * TODO: Reply to the correct user fifo
     * TODO: Log reply to SERVER_LOGFILE
     * TODO: Handle all pending requests before exiting when receiving shutdown request
     */

    /* Server FIFO removal */
    remove_fifo(SERVER_FIFO_PATH);

    exit(EXIT_SUCCESS);
} 
