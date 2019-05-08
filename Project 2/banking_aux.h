#define MAXLINE 512
#include <stdbool.h>
#include "types.h"
#include "sope.h"


bool build_tlv_request(tlv_request_t *request, char*argv[]);

void create_bank_account (bank_account_t *acc, char*password, int acc_id, int balance);
