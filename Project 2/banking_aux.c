#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#include "constants.h"
#include "banking_aux.h"
#include "general_aux.h"

bool create_bank_account (bank_account_t acc[], char*password, int acc_id, int balance){
    if (strlen(acc[acc_id].hash) != 0)
        return false;
    acc[acc_id].account_id = acc_id;
    acc[acc_id].balance = balance;
    generate_password_salt(acc[acc_id].salt);
    generate_sha256_hash(password, acc[acc_id].salt, acc[acc_id].hash);
    return true;
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
                free(args);
                return false;
            }
            request->value.transfer.account_id = atoi(args[0]);
            request->value.transfer.amount = atoi(args[1]);
            break;
        }
        default: break;
    }
    free (args);
    return true;
}

void handle_request (tlv_request_t *request, bank_account_t accounts[], tlv_reply_t *reply){

    switch(request->type){
        case OP_CREATE_ACCOUNT: {
            //Verify if request account id is ADMIN_ACCOUNT_ID   
            // create_bank_accout returns true if account creation was possible false otherwise
            break;
        }

        case OP_BALANCE: {

            break;
        }

        case OP_TRANSFER: {

            break;
        }


        case OP_SHUTDOWN: {
            //Verify if request account id is ADMIN_ACCOUNT_ID 
            break;
        }
        default:break;
    }

}

