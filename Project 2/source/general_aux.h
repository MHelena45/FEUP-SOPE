#include <semaphore.h>
#include <stdbool.h>
#include "sope.h"
#include "types.h"
#define MAXLINE 512

void create_fifo(char *fifo_name);
void remove_fifo(char *fifo_name);
int open_user_fifo(pid_t pid, int flag);
void get_user_fifo_path(pid_t pid, char *fifo_path);

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

void lock_mutex(pthread_mutex_t *mutex, int id, sync_role_t role, int sid);
void unlock_mutex(pthread_mutex_t *mutex, int id, sync_role_t role, int sid);

void init_sem(sem_t *sem, int id, sync_role_t role, int sid, int val);
void wait_sem(sem_t *sem, int id, sync_role_t role, int sid);
void post_sem(sem_t *sem, int id, sync_role_t role, int sid);

void sync_delay(int id, int sid, uint32_t delay_ms);
void shutdown_delay(uint32_t delay_ms);
