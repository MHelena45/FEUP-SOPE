#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <sys/wait.h>
#include "forensic.h"
#include "forensic_aux.h"

struct Options options;

void sig_handler(int signo) //handler do sinal
{
    static int nDirectory = 0;
    static int nFiles = 0;

    switch(signo){
        case SIGUSR1:{
            nDirectory++;
            if (options.o_command != NULL) {
                char mess[100];
                sprintf(mess, "New directory: %i/ %i directories/files at this time.\n", nDirectory, nFiles);
                append_to_file(mess, options.o_command);
            }
            break;
        }
        case SIGUSR2: {
            nFiles++;
            break;
        }
        case SIGINT: exit(0);
    }
}

void getAllHashModes(char *fileChar, char *result) {
    char command[MAXLINE] = "";
    char md5Hash[64];
    char sha1Hash[64];
    char sha256Hash[64];
    char stripedHash[128];

    if(options.hashmode & MD5) {
        memset(command,0,MAXLINE);
        options.mac_mode ? strcat(command,MD5CMDMAC) : strcat(command,MD5CMD);
        strcat(command,fileChar);
        executeSystemCommand(command,md5Hash);
        stripHashCodeFromResult(md5Hash,stripedHash);
        strcat(result,",");
        strcat(result,stripedHash);
    }

    if(options.hashmode & SHA1) {
        memset(command,0,MAXLINE);
        options.mac_mode ? strcat(command,SHA1CMDMAC) : strcat(command,SHA1CMD);
        strcat(command,fileChar);
        executeSystemCommand(command,sha1Hash);
        stripHashCodeFromResult(sha1Hash,stripedHash);
        strcat(result,",");
        strcat(result,stripedHash);
    }

    if(options.hashmode & SHA256) {
        memset(command,0,MAXLINE);
        options.mac_mode ? strcat(command,SHA256CMDMAC) : strcat(command,SHA256CMD);
        strcat(command,fileChar);
        executeSystemCommand(command,sha256Hash);
        stripHashCodeFromResult(sha256Hash,stripedHash);
        strcat(result,",");
        strcat(result,sha256Hash);
    }
}

void analyze_file (char *filepath, struct stat *statdata){

    kill(options.parent_id, SIGUSR2);

    struct tm tst;

    //Modification Time
    char mod_time[19];
    localtime_r(&statdata->st_mtime, &tst);
    sprintf(mod_time, "%d-%d-%dT%d:%d:%d", 1900 + tst.tm_year, tst.tm_mon, tst.tm_mday, tst.tm_hour, tst.tm_min, tst.tm_sec);

    //Access Time
    char acc_time[19];
    localtime_r(&statdata->st_ctime, &tst);
    sprintf(acc_time, "%d-%d-%dT%d:%d:%d", 1900 + tst.tm_year, tst.tm_mon, tst.tm_mday, tst.tm_hour, tst.tm_min, tst.tm_sec);

    //Final output
    char out_message[512];
    sprintf(out_message, "%s,file_type,%ld,file access (parse st_mode),%s,%s", filepath, statdata->st_size, acc_time, mod_time);

    getAllHashModes(filepath,out_message);

    strcat(out_message,"\n");

    if (options.o_command == NULL)
        printf("%s", out_message);
    else append_to_file(out_message, options.o_command);
}

void analyze_path (char *filepath) {

    struct stat statdata;

    if (stat(filepath, &statdata) < 0){
        perror("Error");
        exit(1);
    }

    if (S_ISDIR(statdata.st_mode)){ //Directory

        kill(options.parent_id, SIGUSR1);

        DIR *c_dir;
        struct dirent *dir;
        char temp_filename[257];
        if ( (c_dir = opendir(filepath)) == NULL ){
            perror("Error");
            exit(1);
        }

        while ( (dir = readdir(c_dir)) != NULL)  { //Read files first

            struct stat temp_stat;
            sprintf(temp_filename, "%s/%s", filepath, dir->d_name);

            //Check if path exists
            if (stat(temp_filename, &temp_stat) < 0){
                perror("Error");
                exit(1);
            }

            //Check if regular file
            if (S_ISREG(temp_stat.st_mode))
                analyze_file(temp_filename, &temp_stat);
        }

        c_dir = opendir(filepath);

        while ( (dir = readdir(c_dir)) != NULL && options.r_command)  { //Read Directories after

            struct stat temp_stat;
            sprintf(temp_filename, "%s/%s", filepath, dir->d_name);

            //Check if path exists
            if (stat(temp_filename, &temp_stat) < 0){
                perror("Error");
                exit(1);
            }
            //Check if Directory
            if (S_ISDIR(temp_stat.st_mode) && dir->d_name[0] != '.' ){
                if (fork() > 0){
                    wait(NULL);
                    continue;
                }
                else {
                    analyze_path(temp_filename);
                    exit(0);
                }
            }
        }
    }
    else { //Single file
        analyze_file(filepath, &statdata);
    }
}


int main (int argc, char *argv[], char *envp[]){

    options.hashmode = 0b000;
    options.o_command = NULL;
    options.r_command = false;
    options.v_command = false;
    options.mac_mode = false;
    options.parent_id = getpid();
    char* filepath;

    struct sigaction action;
    action.sa_handler = sig_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    //Signal Handler installer
    if ( (sigaction(SIGINT,&action,NULL) < 0) || (sigaction(SIGUSR1,&action,NULL) < 0) || (sigaction(SIGUSR2,&action,NULL) < 0) ) {
        fprintf(stderr,"Unable to install signal handler\n");
        exit(1);
    }

    if (argc < 2){	/* Needs at least one argument */
        printf("forensic [-r] [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-v] <file|dir>\n");
        exit(1);
    }

    for (int i = 1; i < argc; ++i){	/* Parse arguments */

        if (!strcmp(argv[i], "-r")){
            options.r_command = true;
            continue;
        }

        if (!strcmp(argv[i], "-h")){
            if (i < argc - 2) {
                char *ptr = strtok(argv[i + 1], ",");
                while (ptr != NULL) {
                    if (!checkHashMode(ptr, &options.hashmode)) {
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
            options.v_command = true;
            continue;
        }

        if (!strcmp(argv[i], "-o")){
            if (i < argc - 2){
                options.o_command = argv[i+1];
                remove(options.o_command); //Delete file if it already exists
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
                options.mac_mode = true;
                ++i;
            } else {
                printf("forensic [-r] [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-v] <file|dir>\n");
                exit(1);
            }
        }
    }

    if(options.mac_mode)
        filepath = argv[argc-2];
    else filepath = argv[argc-1];

    analyze_path(filepath);

    if (options.o_command != NULL)
        printf("Data saved on file %s\n", options.o_command);

    exit(0);
}