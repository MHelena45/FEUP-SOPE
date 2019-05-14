/* User  */
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

static char user_fifo_path[USER_FIFO_PATH_LEN];

void exit_handler() {
  struct stat user_fifo;
  if (strlen(user_fifo_path) > 0 && stat(user_fifo_path, &user_fifo) == 0)
    remove_fifo(user_fifo_path);
}

int main(int argc, char* argv[]) {
  /** Initiate exit handler **/
  atexit(exit_handler);

  /** Verify command **/
  if (argc < 6) {
    printf(
        "user [Account ID] [Account Password] [Delay in ms] [Operation code] "
        "[Argument list]\n");
    exit(-1);
  }
  if (!is_valid_password(argv[2])) {
    printf("Password needs to have between %d and %d characters\n",
           MIN_PASSWORD_LEN, MAX_PASSWORD_LEN);
    exit(EXIT_FAILURE);
  }

  /** Build request struct **/
  tlv_request_t request;
  if (!build_tlv_request(&request, argv)) exit(EXIT_FAILURE);

  /** Initiate reply **/
  tlv_reply_t reply;
  reply.type = request.type;
  reply.value.header.account_id = request.value.header.account_id;

  /** Log Request **/
  log_request(USER_LOGFILE, request.value.header.pid, &request);

  /** Write request to server fifo **/
  int server_fifo_fd = open(SERVER_FIFO_PATH, O_WRONLY);
  if (server_fifo_fd == -1) {
    reply.value.header.ret_code = RC_SRV_DOWN;
    log_reply(USER_LOGFILE, request.value.header.pid, &reply);
    exit(EXIT_FAILURE);
  }
  write(server_fifo_fd, &request, sizeof(request));
  close(server_fifo_fd);

  /** Create user FIFO **/
  get_user_fifo_path(request.value.header.pid, user_fifo_path);
  create_fifo(user_fifo_path);
  int user_fifo_fd = open(user_fifo_path, O_RDONLY);
  if (user_fifo_fd == -1) {
    printf("fifo '%s' can not be opened\n", user_fifo_path);
    exit(EXIT_FAILURE);
  }
  /** Wait for server response **/
  clock_t start_t = clock();

  while (((double)(clock() - start_t) / CLOCKS_PER_SEC) < FIFO_TIMEOUT_SECS) {
    if (read(user_fifo_fd, &reply, sizeof(reply)) > 0) {
      log_reply(USER_LOGFILE, request.value.header.pid, &reply);
      close(user_fifo_fd);
      exit(EXIT_SUCCESS);
    }
  }

  reply.value.header.ret_code = RC_SRV_TIMEOUT;
  log_reply(USER_LOGFILE, request.value.header.pid, &reply);
  close(user_fifo_fd);
  exit(EXIT_SUCCESS);
}
