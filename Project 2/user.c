/* User  */
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <string.h>
#include <time.h>

#include "banking_aux.h"
#include "types.h"
#include "sope.h"
#include "constants.h"
#include "general_aux.h"


int main(int argc, char *argv[]){

    /** Verify command **/
    if (argc < 6){
        printf("user [Account ID] [Account Password] [Delay in ms] [Operation code] [Argument list]\n");
        exit(-1);
    }
    if (!is_valid_password(argv[2])){
        printf ("Password needs to have between %d and %d characters\n", MIN_PASSWORD_LEN, MAX_PASSWORD_LEN);
        exit(EXIT_FAILURE);
    }
    
    /** Build request struct **/
    tlv_request_t request;
    if (!build_tlv_request(&request, argv))
        exit(EXIT_FAILURE);
    
    /** Log Request **/
    log_request(USER_LOGFILE, &request);

    /** Write request to server fifo **/
    int server_fifo_fd = open(SERVER_FIFO_PATH, O_WRONLY);
    if (server_fifo_fd == -1){
        printf("fifo '%s' is not available\n", SERVER_FIFO_PATH);
        exit(EXIT_FAILURE);
    }
    write(server_fifo_fd, &request, sizeof(request));
    close(server_fifo_fd);

    /** Create user FIFO **/
    char user_fifo_path[USER_FIFO_PATH_LEN];
    get_user_fifo_path(request.value.header.pid, user_fifo_path);
    create_fifo(user_fifo_path);
    int user_fifo_fd = open(user_fifo_path, O_RDONLY);
    if (user_fifo_fd == -1){
        printf("fifo '%s' is not available\n", user_fifo_path);
        exit(EXIT_FAILURE);
    }

    /** Wait for server response **/ 
    clock_t start_t = clock();
    tlv_reply_t reply;
    while ( ((double)(clock()-start_t) / CLOCKS_PER_SEC) < FIFO_TIMEOUT_SECS){
        if (read(user_fifo_fd, &reply, sizeof(reply)) > 0){
            log_reply(USER_LOGFILE, request.value.header.pid, &reply);
            break;
        }
    } 

    /** Cleanup **/
    remove_fifo(user_fifo_path);
    exit(EXIT_SUCCESS);
} 
