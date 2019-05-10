#define MAXLINE 512
#include <stdbool.h>
#include "types.h"
#include "sope.h"

bool is_active_account(bank_account_t acc[], int acc_id);
bool create_bank_account (bank_account_t acc[], req_create_account_t *acc_data);
enum ret_code validate_bank_account (bank_account_t accounts[], req_header_t *header);
bool build_tlv_request(tlv_request_t *request, char*argv[]);
void build_tlv_reply (tlv_request_t *request, bank_account_t accounts[], tlv_reply_t *reply);


