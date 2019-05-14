#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <time.h>

#include "constants.h"
#include "general_aux.h"

void create_fifo(char* fifo_name) {
  if (mkfifo(fifo_name, 0660) < 0) {
    if (errno == EEXIST)
      printf("FIFO '%s' already exists\n", fifo_name);
    else
      printf("Can't create FIFO '%s'\n", fifo_name);
    exit(EXIT_FAILURE);
  }
}

void remove_fifo(char* fifo_name) {
  if (unlink(fifo_name) < 0) {
    printf("Error when destroying FIFO '%s'\n", fifo_name);
    exit(EXIT_FAILURE);
  }
}

void get_user_fifo_path(int id, char* fifo_name) {
  sprintf(fifo_name, "%s%0*d", USER_FIFO_PATH_PREFIX, WIDTH_ID, id);
}

void run_pipe_command(char* command, char* result) {
  FILE* fpout;
  fpout = popen(command, "r");
  if (fpout == NULL) {
    fprintf(stderr, "Error opening pipe");
    exit(EXIT_FAILURE);
  }
  fgets(result, MAXLINE, fpout);
  if (pclose(fpout) == -1) {
    fprintf(stderr, "Error closing pipe");
  }
}

bool is_valid_password(char* password) {
  if (strlen(password) < MIN_PASSWORD_LEN ||
      strlen(password) > MAX_PASSWORD_LEN)
    return false;
  return true;
}

int get_string_arguments(char* arguments, char* argv[]) {
  int argc = 0;
  char* ptr = strtok(arguments, " ");
  while (ptr != NULL) {
    argv[argc] = ptr;
    ptr = strtok(NULL, " ");
    ++argc;
  }
  return argc;
}

void generate_sha256_hash(char* password, char salt[], char hash[]) {
  char sha256sum_command[MAXLINE];
  sprintf(sha256sum_command, "echo -n \"%s%s\" | sha256sum", password, salt);
  char pipe_result[MAXLINE];
  run_pipe_command(sha256sum_command, pipe_result);
  char* temp_ptr = strtok(pipe_result, " ");
  strcpy(hash, temp_ptr);
  hash[HASH_LEN] = '\0';
}

void generate_password_salt(char salt[]) {
  for (int i = 0; i < SALT_LEN; ++i) {
    int randomValue = rand() % 15;
    sprintf(&salt[i], "%x", randomValue);
  }
  salt[SALT_LEN] = '\0';
}
void log_office_open(int id, pthread_t tid) {
  int log_fd = open(SERVER_LOGFILE, O_CREAT | O_WRONLY | O_APPEND);
  logBankOfficeOpen(log_fd, id, tid);
  close(log_fd);
}
void log_office_close(int id, pthread_t tid) {
  int log_fd = open(SERVER_LOGFILE, O_CREAT | O_WRONLY | O_APPEND);
  logBankOfficeClose(log_fd, id, tid);
  close(log_fd);
}

void log_request(char* log_filename, int id, tlv_request_t* request) {
  int log_fd = open(log_filename, O_CREAT | O_WRONLY | O_APPEND);
  logRequest(log_fd, id, request);
  close(log_fd);
}

void log_account_creation(char* log_filename, int id, bank_account_t* account) {
  int log_fd = open(log_filename, O_CREAT | O_WRONLY | O_APPEND);
  logAccountCreation(log_fd, id, account);
  close(log_fd);
}

void log_reply(char* log_filename, int id, tlv_reply_t* reply) {
  int log_fd = open(log_filename, O_CREAT | O_WRONLY | O_APPEND);
  logReply(log_fd, id, reply);
  close(log_fd);
}

void lock_mutex(pthread_mutex_t* mutex, int id, sync_role_t role, int sid) {
  pthread_mutex_lock(mutex);
  int log_fd = open(SERVER_LOGFILE, O_CREAT | O_WRONLY | O_APPEND);
  logSyncMech(log_fd, id, SYNC_OP_MUTEX_LOCK, role, sid);
  close(log_fd);
}

void unlock_mutex(pthread_mutex_t* mutex, int id, sync_role_t role, int sid) {
  pthread_mutex_unlock(mutex);
  int log_fd = open(SERVER_LOGFILE, O_CREAT | O_WRONLY | O_APPEND);
  logSyncMech(log_fd, id, SYNC_OP_MUTEX_UNLOCK, role, sid);
  close(log_fd);
}

void signal_cond(pthread_cond_t* cond, int id, sync_role_t role, int sid) {
  int log_fd = open(SERVER_LOGFILE, O_CREAT | O_WRONLY | O_APPEND);
  logSyncMech(log_fd, id, SYNC_OP_COND_SIGNAL, role, sid);
  close(log_fd);
  pthread_cond_signal(cond);
}

void log_wait_cond(int id, sync_role_t role, int sid) {
  int log_fd = open(SERVER_LOGFILE, O_CREAT | O_WRONLY | O_APPEND);
  logSyncMech(log_fd, id, SYNC_OP_COND_WAIT, role, sid);
  close(log_fd);
}