#include <stdbool.h>
#include "sope.h"
#include "types.h"
#define MAXLINE 512

void create_fifo(char *fifo_name);
void remove_fifo(char *fifo_name);
void get_user_fifo_path(int id, char *fifo_name);

void run_pipe_command(char *command, char *result);

bool is_valid_password(char *password);
int get_string_arguments(char *arguments, char *argv[]);

void generate_password_salt(char salt[]);
void generate_sha256_hash(char *password, char salt[], char hash[]);

void log_request(char *log_filename, tlv_request_t *request);
void log_account_creation(char *log_filename, int id, bank_account_t *account);
void log_reply(char *log_filename, int id, tlv_reply_t *reply);
