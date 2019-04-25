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

int main(int argc, char *argv[]){

    /**
     * Verify command
     */
    if (argc < 6){
        printf("user [Account ID] [Account Password] [Delay in ms] [Operation code] [Argument list]\n");
        exit(-1);
    }
    if (!is_valid_password(argv[2])){
        printf ("Password needs to have between %d and %d characters\n", MIN_PASSWORD_LEN, MAX_PASSWORD_LEN);
        exit(-1);
    }
    /**
     * Build request
     * TODO: handle operation arguments in get_tlv_request (banking_aux.c)
     */
    char* arguments = argv[5];
    tlv_request_t request;
    get_tlv_request(&request, argv);
    /**
     * Log Request
     */
    int fd = open(USER_LOGFILE, O_CREAT | O_WRONLY);
    logRequest(fd, request.value.header.pid, &request);
    close(fd);
    /**
     * Create user FIFO
     */
    char *fifo_path = (char*)malloc(USER_FIFO_PATH_LEN);
    sprintf(fifo_path, "%s%0*d",USER_FIFO_PATH_PREFIX, WIDTH_ID, request.value.header.pid);
    create_fifo(fifo_path);

    /**
     * TODO: Write to request to /tmp/secure_srv
     * TODO: Wait for server reply for FIFO_TIMEOUT_SECS in constants.h
     * TODO: Build and log tlv_reply_t from server response to the user FIFO
     */ 

    /**
     * Remove user FIFO
     */
    remove_fifo(fifo_path);

exit(0);
} 
