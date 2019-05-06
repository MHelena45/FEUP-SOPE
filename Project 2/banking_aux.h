#define MAXLINE 512
#include <stdbool.h>
#include "types.h"
#include "sope.h"

void create_fifo (char* fifo_name);

void remove_fifo (char *fifo_name);

void run_pipe_command(char *command, char *arguments ,char *result);

bool is_valid_password(char *password);

bool build_tlv_request(tlv_request_t *request, char*argv[]);

int get_string_arguments(char* arguments, char*argv[]);
