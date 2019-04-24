#define MAXLINE 512
#include <stdbool.h>

void create_fifo (char* fifo_name);

void append_to_file(char *message, char *filepath);

void executeSystemCommand(char *command, char *arguments ,char *result);

void remove_fifo (char *fifo_name);

bool is_valid_password(char *password);

