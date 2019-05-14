/*   servidor      */
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <time.h>

#include "banking_aux.h"
#include "constants.h"
#include "general_aux.h"
#include "sope.h"
#include "types.h"

bool request_waiting = false;
bool server_exit = false;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

bank_account_t accounts[MAX_BANK_ACCOUNTS];
tlv_request_t request;

void exit_handler() {
  struct stat server_fifo;
  if (stat(SERVER_FIFO_PATH, &server_fifo) == 0) {
    remove_fifo(SERVER_FIFO_PATH);
  }
}

void *read_request(void *arg) {
  int thread_id = *(int *)arg;

  while (!server_exit) {
    lock_mutex(&mutex, thread_id, SYNC_ROLE_CONSUMER, MAIN_THREAD_ID);
    log_wait_cond(thread_id, SYNC_ROLE_CONSUMER, MAIN_THREAD_ID);
    while (!request_waiting) {
      pthread_cond_wait(&cond, &mutex);
    }

    request_waiting = false;

    tlv_reply_t reply;
    build_tlv_reply(&request, accounts, &reply);

    char fifo_path[USER_FIFO_PATH_LEN];
    get_user_fifo_path(request.value.header.pid, fifo_path);
    int user_fifo_fd = open(fifo_path, O_WRONLY);
    if (user_fifo_fd == -1)
      reply.value.header.ret_code = RC_USR_DOWN;
    else
      write(user_fifo_fd, &reply, sizeof(reply));

    log_reply(SERVER_LOGFILE, MAIN_THREAD_ID, &reply);
    memset(&reply, 0, sizeof(reply));
    close(user_fifo_fd);
    unlock_mutex(&mutex, thread_id, SYNC_ROLE_CONSUMER,
                 request.value.header.pid);
  }
  return NULL;
}

int main(int argc, char *argv[]) {
  /** Initiate exit handler **/
  atexit(exit_handler);

  /** Verify command **/
  if (argc < 3) {
    printf("server [Number of bank offices] [Administrator password]\n");
    exit(EXIT_FAILURE);
  }
  int threads_number = atoi(argv[1]);
  if (threads_number > MAX_BANK_OFFICES) {
    printf("The maximum number of bank offices is %d\n", MAX_BANK_OFFICES);
    exit(EXIT_FAILURE);
  }
  char *admin_password = argv[2];
  if (!is_valid_password(admin_password)) {
    printf("Password needs to have between %d and %d characters\n",
           MIN_PASSWORD_LEN, MAX_PASSWORD_LEN);
    exit(EXIT_FAILURE);
  }
  /** Initiate random seed **/
  srand(time(NULL));
  /** Initiate threads**/
  pthread_t threads[threads_number];  // TODO: Create server threads
  int threadsNumber[threads_number];
  for (int i = 0; i < threads_number; i++) {
    threadsNumber[i] = i + 1;
    threads[i] = pthread_create(&threads[i], NULL, read_request,
                                (void *)&threadsNumber[i]);
  }
  /** Create admin account **/
  req_create_account_t admin_account;
  admin_account.account_id = ADMIN_ACCOUNT_ID;
  strcpy(admin_account.password, admin_password);
  admin_account.balance = 0;
  create_bank_account(accounts, &admin_account);

  /** Server FIFO creation **/
  create_fifo(SERVER_FIFO_PATH);
  op_type_t operation = 0;

  int server_fifo_fd = open(SERVER_FIFO_PATH, O_RDONLY);
  if (server_fifo_fd == -1) {
    printf("fifo '%s' is not available\n", SERVER_FIFO_PATH);
    exit(EXIT_FAILURE);
  }

  while (operation != OP_SHUTDOWN) {
    if (read(server_fifo_fd, &request, sizeof(request)) > 0) {
      log_request(SERVER_LOGFILE, &request);
      lock_mutex(&mutex, MAIN_THREAD_ID, SYNC_ROLE_PRODUCER,
                 request.value.header.pid);
      request_waiting = true;
      signal_cond(&cond, MAIN_THREAD_ID, SYNC_ROLE_PRODUCER,
                  request.value.header.pid);
      operation = request.type;
      unlock_mutex(&mutex, MAIN_THREAD_ID, SYNC_ROLE_PRODUCER,
                   request.value.header.pid);
    }
  }

  close(server_fifo_fd);
  exit(EXIT_SUCCESS);
}
