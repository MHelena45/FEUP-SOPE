#include <stdbool.h>
#include "types.h"
#include "sope.h"
#define MAXLINE 512

void create_fifo (char* fifo_name);
void remove_fifo (char *fifo_name);

void run_pipe_command(char *command, char *result);

bool is_valid_password(char *password);

int get_string_arguments(char* arguments, char*argv[]);

void generate_password_salt(char salt[]);
void generate_sha256_hash(char *password, char salt[], char hash[]);