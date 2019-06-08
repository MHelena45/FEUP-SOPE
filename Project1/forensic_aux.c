#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "forensic_aux.h"

void append_to_file(char *message, char *filepath){
    FILE *out_file;
    out_file = fopen (filepath, "a");
    if (out_file != NULL){
        fputs(message, out_file);
        fclose(out_file);
    }
}

void executeSystemCommand(char *command, char *arguments ,char *result) {

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

bool checkHashMode(char *hashMode, uint8_t *hash_option) {
    *hash_option |= !strcmp(hashMode,"md5") ? MD5:0;
    *hash_option |= !strcmp(hashMode,"sha1") ? SHA1:0;
    *hash_option |= !strcmp(hashMode,"sha256") ? SHA256:0;

    return *hash_option;
}

void stripHashCodeFromResult(char *hashCodeResult, char *stripedHash) {
    memset(stripedHash,0,128);
    //This is just for MacOs, because MD5 has a different output
    char *pointerPosition = strchr(hashCodeResult,'=');
    if (pointerPosition != NULL) {
        strcat(stripedHash,pointerPosition+2);
        int i = 0;
        while(stripedHash[i] != '\n') {
            ++i;
        }
        stripedHash[i] = '\0';
        return;
    }
    pointerPosition = strtok(hashCodeResult," ");
    strcat(stripedHash,pointerPosition);
}

void parse_arguments(int argc, char **argv, Options *options){

    if (argc < 2){	/* Needs at least one argument */
        printf("forensic [-r] [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-v] <file|dir>\n");
        exit(1);
    }

    for (int i = 1; i < argc; ++i){	/* Parse arguments */

        if (!strcmp(argv[i], "-r")){
            options->r_command = true;
            continue;
        }

        if (!strcmp(argv[i], "-h")){
            if (i < argc - 2) {
                char *ptr = strtok(argv[i + 1], ",");
                while (ptr != NULL) {
                    if (!checkHashMode(ptr, &options->hashmode)) {
                        printf("Invalid parameter for hash function: [md5,sha1,sha256]\n");
                        exit(1);
                    }
                    ptr = strtok(NULL,",");
                }
                ++i;
                continue;
            } else {
                printf("forensic [-r] [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-v] <file|dir>\n");
                exit(1);
            }
        }

        if (!strcmp(argv[i], "-v")){
            options->v_command = true;
            continue;
        }

        if (!strcmp(argv[i], "-o")){
            if (i < argc - 2){
                options->o_command = argv[i+1];
                remove(options->o_command); //Delete file if it already exists
                ++i;
                continue;
            }
            else {
                printf("forensic [-r] [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-v] <file|dir>\n");
                exit(1);
            }
        }

        if(!strcmp(argv[i], "-mac")) {
            if (i == argc - 1) {
                options->mac_mode = true;
                ++i;
            } else {
                printf("forensic [-r] [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-v] <file|dir>\n");
                exit(1);
            }
        }
    }
}

void convert_timespec_to_string(time_t *org, char *dest){
    struct tm tst;

    //Modification Time
    localtime_r(org, &tst);
    sprintf(dest, "%d-%d-%dT%d:%d:%d", 1900 + tst.tm_year, tst.tm_mon, tst.tm_mday, tst.tm_hour, tst.tm_min, tst.tm_sec);
}



