#include "banking_aux.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <string.h> 
#include <errno.h>

void create_fifo (char* fifo_name){
    if (mkfifo(fifo_name, 0660) < 0){
        if (errno==EEXIST) printf("FIFO '%s' already exists\n", fifo_name);
        else printf("Can't create FIFO '%s'\n", fifo_name);
        exit(-1);
    }
}

void remove_fifo (char *fifo_name){
    if (unlink("/tmp/requests")<0){
        printf("Error when destroying FIFO '%s'\n", fifo_name);
        exit(-1);
    }
}

void append_to_file(char *message, char *filepath){
    FILE *out_file;
    out_file = fopen (filepath, "a");
    if (out_file != NULL){
        fputs(message, out_file);
        fclose(out_file);
    }
}

void run_pipe_command(char *command, char *arguments ,char *result) {
    char cmd[260];
    sprintf(cmd, "%s \"%s\"", command, arguments);

    int pipeStatus;
    FILE *fpout;
    fpout = popen(cmd, "r");
    if (fpout == NULL) {
        fprintf(stderr, "Error opening pipe");
        exit(1);
    }
    fgets(result, MAXLINE, fpout);
    pipeStatus = pclose(fpout);
    if(pipeStatus == -1) {
        fprintf(stderr, "Error closing pipe");
    }
}
