/*   servidor      */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <pthread.h>
#include <time.h>

#include "banking_aux.h"
#include "types.h"
#include "sope.h"
#include "constants.h"
#include "general_aux.h"

bank_account_t accounts[MAX_BANK_ACCOUNTS];

void exit_handler(){
    struct stat server_fifo;
    if (stat (SERVER_FIFO_PATH, &server_fifo) == 0){
        remove_fifo(SERVER_FIFO_PATH);
    }
}

void read_request(void *req){
    tlv_request_t *request = (tlv_request_t *) req;
    log_request(SERVER_LOGFILE, request);

    tlv_reply_t reply;
    build_tlv_reply(request, accounts, &reply);

    char fifo_path[USER_FIFO_PATH_LEN];
    get_user_fifo_path(request->value.header.pid, fifo_path);
    int user_fifo_fd = open(fifo_path, O_WRONLY);
    if (user_fifo_fd == -1){
        reply.value.header.ret_code = RC_USR_DOWN;
        log_reply(SERVER_LOGFILE, MAIN_THREAD_ID, &reply);
        return;
    }

    log_reply(SERVER_LOGFILE, MAIN_THREAD_ID, &reply);
    write(user_fifo_fd, &reply, sizeof(reply));
    close (user_fifo_fd);
    memset(&reply, 0, sizeof(reply));
}

int main(int argc, char *argv[]){
    /** Initiate exit handler **/
    atexit(exit_handler);

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
        printf("Password needs to have between %d and %d characters\n", MIN_PASSWORD_LEN, MAX_PASSWORD_LEN);
        exit(EXIT_FAILURE);
    }
    /** Initiate random seed **/
    srand(time(NULL));
    /** Initiate threads**/
    pthread_t threads[threads_number]; //TODO: Create server threads
    /** Create admin account **/
    req_create_account_t admin_account;
    admin_account.account_id = ADMIN_ACCOUNT_ID;
    strcpy(admin_account.password, admin_password);
    admin_account.balance = 0;
    create_bank_account(accounts, &admin_account);
    log_account_creation(SERVER_LOGFILE, MAIN_THREAD_ID, &accounts[ADMIN_ACCOUNT_ID]);

    /** Server FIFO creation **/
    create_fifo(SERVER_FIFO_PATH);
    /** 
     * ! Single counter/thread for now 
     */
    op_type_t operation = 0;
    tlv_request_t request;
    int server_fifo_fd = open(SERVER_FIFO_PATH, O_RDONLY);
    if (server_fifo_fd == -1){
        printf("fifo '%s' is not available\n", SERVER_FIFO_PATH);
        exit(EXIT_FAILURE);
    }

    while (operation != OP_SHUTDOWN) {
        if (read(server_fifo_fd, &request, sizeof(request)) > 0){
            operation = request.type;
            read_request((void *) &request);
        }
    }
    close(server_fifo_fd);
    exit(EXIT_SUCCESS);
}
