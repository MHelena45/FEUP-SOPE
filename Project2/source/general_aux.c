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

#define NOT_SHARED 0

void create_fifo(char* fifo_name) {
  if (mkfifo(fifo_name, 0660) < 0) {
    if (errno == EEXIST) {
      remove_fifo(fifo_name);
      create_fifo(fifo_name);
    } else {
      printf("Can't create FIFO '%s'\n", fifo_name);
      exit(EXIT_FAILURE);
    }
  }
}

void remove_fifo(char* fifo_name) {
  if (unlink(fifo_name) < 0) {
    printf("Error when destroying FIFO '%s'\n", fifo_name);
    exit(EXIT_FAILURE);
  }
}

int open_user_fifo(pid_t pid, int flag) {
  char fifo_path[USER_FIFO_PATH_LEN];
  sprintf(fifo_path, "%s%0*d", USER_FIFO_PATH_PREFIX, WIDTH_ID, (int)pid);
  return open(fifo_path, flag);
}
void get_user_fifo_path(pid_t pid, char* fifo_path) {
  sprintf(fifo_path, "%s%0*d", USER_FIFO_PATH_PREFIX, WIDTH_ID, (int)pid);
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

void generate_password_salt(char salt[]) {
  for (int i = 0; i < SALT_LEN; ++i) {
    int randomValue = rand() % 15;
    sprintf(&salt[i], "%x", randomValue);
  }
  salt[SALT_LEN] = '\0';
}

void generate_sha256_hash(char* password, char salt[], char hash[]) {
  char sha256sum_command[MAXLINE];
  sprintf(sha256sum_command, "echo -n \"%s%s\" | shasum -a 256", password,
          salt);
  char pipe_result[MAXLINE];
  run_pipe_command(sha256sum_command, pipe_result);
  char* temp_ptr = strtok(pipe_result, " ");
  strcpy(hash, temp_ptr);
  hash[HASH_LEN] = '\0';
}

void log_office_open(int id, pthread_t tid) {
  int log_fd = open(SERVER_LOGFILE, O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
  logBankOfficeOpen(log_fd, id, tid);
  close(log_fd);
}
void log_office_close(int id, pthread_t tid) {
  int log_fd = open(SERVER_LOGFILE, O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
  logBankOfficeClose(log_fd, id, tid);
  close(log_fd);
}

void log_request(char* log_filename, int id, tlv_request_t* request) {
  int log_fd = open(log_filename, O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
  logRequest(log_fd, id, request);
  close(log_fd);
}

void log_account_creation(char* log_filename, int id, bank_account_t* account) {
  int log_fd = open(log_filename, O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
  logAccountCreation(log_fd, id, account);
  close(log_fd);
}

void log_reply(char* log_filename, int id, tlv_reply_t* reply) {
  int log_fd = open(log_filename, O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
  logReply(log_fd, id, reply);
  close(log_fd);
}

void lock_mutex(pthread_mutex_t* mutex, int id, sync_role_t role, int sid) {
  pthread_mutex_lock(mutex);
  int log_fd = open(SERVER_LOGFILE, O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
  logSyncMech(log_fd, id, SYNC_OP_MUTEX_LOCK, role, sid);
  close(log_fd);
}

void unlock_mutex(pthread_mutex_t* mutex, int id, sync_role_t role, int sid) {
  pthread_mutex_unlock(mutex);
  int log_fd = open(SERVER_LOGFILE, O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
  logSyncMech(log_fd, id, SYNC_OP_MUTEX_UNLOCK, role, sid);
  close(log_fd);
}

void init_sem(sem_t* sem, int id, sync_role_t role, int sid, int val) {
  int log_fd = open(SERVER_LOGFILE, O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
  logSyncMechSem(log_fd, id, SYNC_OP_SEM_INIT, role, sid, val);
  close(log_fd);
  sem_init(sem, NOT_SHARED, val);
}

void wait_sem(sem_t* sem, int id, sync_role_t role, int sid) {
  int sem_val;
  sem_getvalue(sem, &sem_val);
  int log_fd = open(SERVER_LOGFILE, O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
  logSyncMechSem(log_fd, id, SYNC_OP_SEM_WAIT, role, sid, sem_val);
  close(log_fd);
  sem_wait(sem);
}

void post_sem(sem_t* sem, int id, sync_role_t role, int sid) {
  int sem_val;
  sem_getvalue(sem, &sem_val);
  int log_fd = open(SERVER_LOGFILE, O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
  logSyncMechSem(log_fd, id, SYNC_OP_SEM_POST, role, sid, sem_val);
  close(log_fd);
  sem_post(sem);
}

void sync_delay(int id, int sid, uint32_t delay_ms) {
  int log_fd = open(SERVER_LOGFILE, O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
  logSyncDelay(log_fd, id, sid, delay_ms);
  close(log_fd);
  usleep(delay_ms * 1000);
}

void shutdown_delay(uint32_t delay_ms) {
  int log_fd = open(SERVER_LOGFILE, O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
  logDelay(log_fd, MAIN_THREAD_ID, delay_ms);
  close(log_fd);
  usleep(delay_ms * 1000);
}
