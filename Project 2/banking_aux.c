#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <string.h> 
#include <errno.h>

#include "constants.h"
#include "banking_aux.h"

void create_fifo (char* fifo_name){
    if (mkfifo(fifo_name, 0660) < 0){
        if (errno==EEXIST) printf("FIFO '%s' already exists\n", fifo_name);
        else printf("Can't create FIFO '%s'\n", fifo_name);
        exit(-1);
    }
}

void remove_fifo (char *fifo_name){
    if (unlink(fifo_name)<0){
        printf("Error when destroying FIFO '%s'\n", fifo_name);
        exit(-1);
    }
}

void run_pipe_command(char *command, char *arguments ,char *result) {
    char cmd[260];
    sprintf(cmd, "%s \"%s\"", command, arguments);

    int pipeStatus;
    FILE *fpout;
    fpout = popen(cmd, "r");
    if (fpout == NULL) {
        fprintf(stderr, "Error opening pipe");
        exit(1);
    }
    fgets(result, MAXLINE, fpout);
    pipeStatus = pclose(fpout);
    if(pipeStatus == -1) {
        fprintf(stderr, "Error closing pipe");
    }
}

bool is_valid_password(char *password){
    if (strlen(password) < MIN_PASSWORD_LEN)
        return false;    
    if (strlen(password) > MAX_PASSWORD_LEN)
        return false;
    
    return true;
}

int get_string_arguments(char* arguments, char*argv[]){
    int argc = 0;
    char *ptr = strtok(arguments, " ");
    while (ptr != NULL){
        argv[argc] = ptr;
        ptr = strtok(NULL, " ");
        ++argc;
    }
    return argc;
}

bool build_tlv_request(tlv_request_t *request, char*argv[]){
    request->type = atoi(argv[4]);
    
    int password_length = strlen(argv[2]);
    memcpy(request->value.header.password, argv[2], sizeof(char) * password_length);
    request->value.header.password[password_length] = '\0'; //Terminate string
    
    request->value.header.pid = getpid();
    request->value.header.op_delay_ms = atoi(argv[3]);
    if (request->value.header.op_delay_ms > MAX_OP_DELAY_MS){
        printf("The maximum value for delay is %d\n", MAX_OP_DELAY_MS);
        return false;
    }
    request->value.header.account_id = atoi(argv[1]);
    request->length = sizeof(request->value);

    char **args = (char**) malloc (strlen(argv[5]));
    int argc = get_string_arguments(argv[5], args);
    switch (request->type){
        case OP_CREATE_ACCOUNT: {
            if (argc < 3){
                printf("To create an account id, balance and password are needed\n");
                return false;
            }
            request->value.create.account_id = atoi(args[0]);
            request->value.create.balance = atoi(args[1]);
            int pw_length = strlen(args[2]);
            memcpy(request->value.create.password, args[2], pw_length);
            request->value.create.password[pw_length] = '\0'; //Terminate string
            break;
        }
        case OP_TRANSFER: {
            if (argc < 2){
                printf("To transfer funds an account id and amount are needed\n");
                return false;
            }
            request->value.transfer.account_id = atoi(args[0]);
            request->value.transfer.amount = atoi(args[1]);
            break;
        }
        default: break;
    }
    return true;
}

