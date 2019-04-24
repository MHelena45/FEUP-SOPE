
#define SERVER_FIFO_NAME "/tmp/secure_srv"
#define MAXLINE 512

void create_fifo (char* fifo_name);

void append_to_file(char *message, char *filepath);

void executeSystemCommand(char *command, char *arguments ,char *result);

void remove_fifo (char *fifo_name);
