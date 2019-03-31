#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "forensic_aux.h"

void append_to_file(char *message, char *filepath){
    FILE *out_file;
    out_file = fopen (filepath, "a");
    if (out_file != NULL){
        fputs(message, out_file);
        fclose(out_file);
    }
}

void executeSystemCommand(char *command, char *result) {
    int pipeStatus;
    FILE *fpout;
    fpout = popen(command, "r");
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
    if (!strcmp(hashMode,"md5")) {
        *hash_option |= MD5;
        return true;
    }

    if (!strcmp(hashMode,"sha1")) {
        *hash_option |= SHA1;
        return true;
    }

    if (!strcmp(hashMode,"sha256")) {
        *hash_option |= SHA256;
        return true;
    }

    return false;
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