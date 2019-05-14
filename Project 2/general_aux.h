#include <stdbool.h>
#include "sope.h"
#include "types.h"
#define MAXLINE 512
extern bool server_exit;
extern int active_threads;

typedef struct server_var {
  bool request_waiting;
  bool server_exit;
  int active_threads;
} __attribute__((packed)) server_var_t;

void create_fifo(char *fifo_name);
void remove_fifo(char *fifo_name);
void get_user_fifo_path(int id, char *fifo_name);

void run_pipe_command(char *command, char *result);

bool is_valid_password(char *password);
int get_string_arguments(char *arguments, char *argv[]);

void generate_password_salt(char salt[]);
void generate_sha256_hash(char *password, char salt[], char hash[]);

void log_office_open(int id, pthread_t tid);
void log_office_close(int id, pthread_t tid);
void log_request(char *log_filename, int id, tlv_request_t *request);
void log_account_creation(char *log_filename, int id, bank_account_t *account);
void log_reply(char *log_filename, int id, tlv_reply_t *reply);
void log_wait_cond(int id, sync_role_t role, int sid);

void lock_mutex(pthread_mutex_t *mutex, int id, sync_role_t role, int sid);
void unlock_mutex(pthread_mutex_t *mutex, int id, sync_role_t role, int sid);
void signal_cond(pthread_cond_t *cond, int id, sync_role_t role, int sid);
