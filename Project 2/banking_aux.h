#define MAXLINE 512
#include <stdbool.h>
#include "semaphore.h"
#include "sope.h"
#include "types.h"

extern bool server_exit;
extern int active_threads;

typedef struct bank_account_sem {
  bank_account_t bankAccount;
  sem_t semaphore;
  bool active;
} bank_account_sem_t;

bool is_active_account(bank_account_sem_t acc[], int acc_id);
bool create_bank_account(bank_account_sem_t acc[],
                         req_create_account_t* acc_data, int id);
ret_code_t validate_bank_account(bank_account_sem_t accounts[],
                                 req_header_t* header);

bool build_tlv_request(tlv_request_t* request, char* argv[]);

void build_tlv_reply(tlv_request_t* request, bank_account_sem_t accounts[],
                     tlv_reply_t* reply, int id);
