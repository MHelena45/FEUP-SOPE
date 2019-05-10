#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#include "constants.h"
#include "banking_aux.h"
#include "general_aux.h"

bool is_active_account(bank_account_t acc[], int acc_id){
    if (strlen(acc[acc_id].hash) == 0)
        return false;
    return true;
}

bool create_bank_account (bank_account_t acc[], req_create_account_t *acc_data){
    int acc_id = acc_data->account_id;
    if (is_active_account(acc, acc_id))
        return false;
    acc[acc_id].account_id = acc_id;
    acc[acc_id].balance = acc_data->balance;
    generate_password_salt(acc[acc_id].salt);
    generate_sha256_hash(acc_data->password, acc[acc_id].salt, acc[acc_id].hash);
    return true;
}

enum ret_code validate_bank_account (bank_account_t accounts[], req_header_t *header){
    if (!is_active_account(accounts, header->account_id))
        return RC_ID_NOT_FOUND;
    char hash[HASH_LEN];
    generate_sha256_hash(header->password, accounts[header->account_id].salt, hash);
    if (strcmp(hash, accounts[header->account_id].hash) != 0)
        return RC_LOGIN_FAIL;
    return RC_OK;
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

void build_tlv_reply (tlv_request_t *request, bank_account_t accounts[], tlv_reply_t *reply){
    reply->type = request->type;
    reply->value.header.account_id = request->value.header.account_id;
    reply->value.header.ret_code = validate_bank_account(accounts, &request->value.header);
    
    if (reply->value.header.ret_code == RC_OK){
        switch(request->type){
            case OP_CREATE_ACCOUNT: {
                /** If user is not admin **/
                if (request->value.header.account_id != ADMIN_ACCOUNT_ID){
                    reply->value.header.ret_code = RC_OP_NALLOW;
                    break;
                }
                /** If Account already exists **/
                if (is_active_account(accounts, request->value.create.account_id)){
                    reply->value.header.ret_code = RC_ID_IN_USE;
                    break;
                }
                /** Create account **/
                create_bank_account(accounts, &request->value.create);
                break;
            }
            case OP_BALANCE: {

                break;
            }
            case OP_TRANSFER: {

                break;
            }
            case OP_SHUTDOWN: {
                /* If user is not admin */
                if (request->value.header.account_id != ADMIN_ACCOUNT_ID){
                    reply->value.header.ret_code = RC_OP_NALLOW;
                    break;
                }
                break;
            }
            default:break;
        }
    }
    reply->length = sizeof(reply);
}

