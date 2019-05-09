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
    
    /** Create user FIFO **/
    char fifo_path[USER_FIFO_PATH_LEN];
    sprintf(fifo_path, "%s%0*d",USER_FIFO_PATH_PREFIX, WIDTH_ID, request.value.header.pid);
    create_fifo(fifo_path);
    
    /** Write request to server fifo **/
    int server_fifo_fd = open_fifo(SERVER_FIFO_PATH, O_WRONLY);
    write(server_fifo_fd, &request, sizeof(request));
    close(server_fifo_fd);

    /** Wait for server response **/ 
    clock_t start_t = clock();
    while ( ((double)(clock()-start_t) / CLOCKS_PER_SEC) < FIFO_TIMEOUT_SECS){
        /**
         * TODO: Check for server response
         * TODO: Log server response
         */
    } 

    /** Remove user FIFO **/
    remove_fifo(fifo_path);

    exit(EXIT_SUCCESS);
} 
