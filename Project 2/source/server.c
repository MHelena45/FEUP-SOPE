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
#include "list_queue.h"
#include "sope.h"
#include "types.h"

uint32_t shutdown_delay_ms;
extern bool server_exit;
bool close_thread = false;
extern int active_threads;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t main_sem;

bank_account_sem_t accounts[MAX_BANK_ACCOUNTS];
list_queue_t requests;

void exit_handler() {
  struct stat server_fifo;
  if (stat(SERVER_FIFO_PATH, &server_fifo) == 0) {
    remove_fifo(SERVER_FIFO_PATH);
  }
}

void *read_request(void *arg) {
  int thread_id = *(int *)arg;

  while (!close_thread) {
    /** Wait for request **/
    wait_sem(&main_sem, thread_id, SYNC_ROLE_CONSUMER, ADMIN_ACCOUNT_ID);

    if (close_thread) break;
    /** Get request from the request queue and mark thread as active **/
    lock_mutex(&mutex, thread_id, SYNC_ROLE_CONSUMER, ADMIN_ACCOUNT_ID);
    tlv_request_t request = requests.front(&requests);
    requests.pop(&requests);
    log_request(SERVER_LOGFILE, thread_id, &request);
    ++active_threads;
    tlv_reply_t reply;
    sem_t *acc_sem, *target_sem;
    acc_sem = &accounts[request.value.header.account_id].semaphore;

    /** Wait for access to requesting account and apply delay **/
    if (accounts[request.value.header.account_id].active) {
      wait_sem(acc_sem, thread_id, SYNC_ROLE_ACCOUNT,
               request.value.header.account_id);
      sync_delay(thread_id, request.value.header.account_id,
                 request.value.header.op_delay_ms);
    }
    /** Wait for access to target account if transfer and apply delay **/
    if (request.type == OP_TRANSFER &&
        accounts[request.value.transfer.account_id].active) {
      target_sem = &accounts[request.value.transfer.account_id].semaphore;
      wait_sem(target_sem, thread_id, SYNC_ROLE_ACCOUNT,
               request.value.transfer.account_id);
      sync_delay(thread_id, request.value.transfer.account_id,
                 request.value.header.op_delay_ms);
    }
    /** Handle request and build tlv_reply_t **/
    build_tlv_reply(&request, accounts, &reply, thread_id);

    /** Unlock accounts **/
    if (accounts[request.value.header.account_id].active)
      post_sem(acc_sem, thread_id, SYNC_ROLE_ACCOUNT,
               request.value.header.account_id);
    if (request.type == OP_TRANSFER &&
        accounts[request.value.transfer.account_id].active)
      post_sem(target_sem, thread_id, SYNC_ROLE_ACCOUNT,
               request.value.transfer.account_id);
    /** Write to user fifo if available **/
    int user_fifo_fd =
        open_user_fifo(request.value.header.pid, O_WRONLY | O_NONBLOCK);
    if (user_fifo_fd == -1)
      reply.value.header.ret_code = RC_USR_DOWN;
    else
      write(user_fifo_fd, &reply, sizeof(reply));

    /** Log reply **/
    log_reply(SERVER_LOGFILE, thread_id, &reply);
    unlock_mutex(&mutex, thread_id, SYNC_ROLE_CONSUMER,
                 request.value.header.pid);

    /** Get program ready to shutdown if shutdown command is sucessful **/
    if (reply.type == OP_SHUTDOWN && reply.value.header.ret_code == RC_OK) {
      chmod(SERVER_FIFO_PATH, S_IRUSR | S_IRGRP | S_IROTH);
      shutdown_delay_ms = request.value.header.op_delay_ms;
      server_exit = true;
    }
    /** Reset thread  **/
    memset(&reply, 0, sizeof(reply));
    --active_threads;
    close(user_fifo_fd);
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

  /** Initiate accounts **/
  for (int i = 0; i < MAX_BANK_ACCOUNTS; ++i) accounts[i].active = false;

  /** Initiate random seed **/
  srand(time(NULL));
  list_queue_init(&requests);
  /** Initiate threads**/
  init_sem(&main_sem, MAIN_THREAD_ID, SYNC_ROLE_PRODUCER, ADMIN_ACCOUNT_ID, 0);
  pthread_t threads[threads_number];
  int thread_ids[threads_number];
  for (int i = 0; i < threads_number; i++) {
    thread_ids[i] = i + 1;
    pthread_create(&threads[i], NULL, read_request, (void *)&thread_ids[i]);
    log_office_open(thread_ids[i], threads[i]);
  }
  /** Create admin account **/
  req_create_account_t admin_account;
  admin_account.account_id = ADMIN_ACCOUNT_ID;
  strcpy(admin_account.password, admin_password);
  admin_account.balance = 0;
  create_bank_account(accounts, &admin_account, MAIN_THREAD_ID);

  /** Server FIFO creation **/
  create_fifo(SERVER_FIFO_PATH);
  int server_fifo_fd = open(SERVER_FIFO_PATH, O_RDONLY);
  if (server_fifo_fd == -1) {
    printf("fifo '%s' is not available\n", SERVER_FIFO_PATH);
    exit(EXIT_FAILURE);
  }

  /** Read and push requests to the queue **/
  tlv_request_t request;
  while (!close_thread) {
    if (server_exit && requests.size == 0) {
      close_thread = true;
    }
    if (read(server_fifo_fd, &request, sizeof(request)) > 0) {
      lock_mutex(&mutex, MAIN_THREAD_ID, SYNC_ROLE_CONSUMER, ADMIN_ACCOUNT_ID);
      requests.push(&requests, &request);
      unlock_mutex(&mutex, MAIN_THREAD_ID, SYNC_ROLE_CONSUMER,
                   request.value.header.pid);
      post_sem(&main_sem, MAIN_THREAD_ID, SYNC_ROLE_PRODUCER,
               request.value.header.pid);
    }
  }
  shutdown_delay(shutdown_delay_ms);
  /** Let all threads exit **/
  for (int i = 0; i < threads_number; i++) {
    post_sem(&main_sem, MAIN_THREAD_ID, SYNC_ROLE_PRODUCER, ADMIN_ACCOUNT_ID);
  }
  /** Make sure all threads terminate **/
  for (int i = 0; i < threads_number; i++) {
    pthread_join(threads[i], NULL);
    log_office_close(thread_ids[i], threads[i]);
  }
  /** Destroy semaphores **/
  sem_destroy(&main_sem);
  for (int i = 0; i < MAX_BANK_ACCOUNTS; ++i) {
    if (is_active_account(accounts, i)) {
      sem_destroy(&accounts[i].semaphore);
    }
  }
  close(server_fifo_fd);
  exit(EXIT_SUCCESS);
}
