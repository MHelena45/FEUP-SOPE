#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdint.h>
#include <sys/wait.h>
#include "forensic.h"
#include "forensic_aux.h"

Options options;

void sig_handler(int signo) //Signal handler
{
    static int nDirectory = 0;
    static int nFiles = 0;

    switch(signo){
        case SIGUSR1:{
            nDirectory++;

            if (options.v_command)
                log_command("SIGNAL SIGUSR1");

            if (options.o_command != NULL) {
                char mess[100];
                sprintf(mess, "New directory: %i/ %i directories/files at this time.\n", nDirectory, nFiles);
                append_to_file(mess, options.o_command);
            }
            break;
        }
        case SIGUSR2: {
            nFiles++;

            if (options.v_command)
                log_command("SIGNAL SIGUSR2");
            break;
        }
        case SIGINT: {
            if (options.v_command)
                log_command("SIGNAL SIGINT");
            exit(0);
        }
    }
}

void log_command(char *command){
    struct timeval time_now;
    gettimeofday(&time_now, NULL);

    double inst = (time_now.tv_usec - options.start_time.tv_usec) / 1000;
    char log_out[512];

    sprintf(log_out, "inst:%0.02f - pid: %d - act: %s\n", inst, getpid(), command);
    append_to_file(log_out, options.log_filepath);
}

void getAllHashModes(char *fileChar, char *result) {
    char md5Hash[64], sha1Hash[64], sha256Hash[64], stripedHash[128];

    if(options.hashmode & MD5) {
        executeSystemCommand( (options.mac_mode ? MD5CMDMAC:MD5CMD), fileChar, md5Hash);
        stripHashCodeFromResult(md5Hash,stripedHash);
        strcat(result,",");
        strcat(result,stripedHash);
    }

    if(options.hashmode & SHA1) {
        executeSystemCommand( (options.mac_mode ? SHA1CMDMAC:SHA1CMD), fileChar ,sha1Hash);
        stripHashCodeFromResult(sha1Hash,stripedHash);
        strcat(result,",");
        strcat(result,stripedHash);
    }

    if(options.hashmode & SHA256) {
        executeSystemCommand( (options.mac_mode ? SHA256CMDMAC:SHA256CMD), fileChar, sha256Hash);
        stripHashCodeFromResult(sha256Hash,stripedHash);
        strcat(result,",");
        strcat(result,sha256Hash);
    }
}

void analyze_file (char *filepath, struct stat *statdata){
    //Signal new file
    if (options.o_command)
        kill(options.parent_id, SIGUSR2);

    //Log analyzed files
    if (options.v_command) {
        char log_msg[255] = "ANALYZED ";
        strcat(log_msg, filepath);
        log_command(log_msg);
    }

    //File Name and Type
    char file_result[260] = "";
    bool comma = false;
    executeSystemCommand("file ", filepath, file_result);
    if (strchr(file_result, ',') != NULL)
        comma = true;
    char *file_name = strtok(file_result, ":");
    char *file_type = comma ? strtok(NULL, ",") : strtok(NULL, "\n");
    ++file_type;

    //File Permissions
    char file_perm[4] = "";
    if (statdata->st_mode & S_IRUSR)
        strcat(file_perm, "r");
    if (statdata->st_mode & S_IWUSR)
        strcat(file_perm, "w");
    if (statdata->st_mode & S_IXUSR)
        strcat(file_perm, "x");

    //Modification and last access times
    char mod_time[19], acc_time[19];
    convert_timespec_to_string(&statdata->st_mtime, mod_time);
    convert_timespec_to_string(&statdata->st_ctime, acc_time);

    //Normal Output
    char out_message[1024];

    //This is to avoid warnings on MacOs.
    //MacOs treats ld has lld
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"
    sprintf(out_message, "%s,%s,%ld,%s,%s,%s", file_name, file_type,statdata->st_size, file_perm, acc_time, mod_time);
#pragma GCC diagnostic pop

    //Add optional hashes
    getAllHashModes(filepath, out_message);

    //Final output
    strcat(out_message,"\n");

    //Print or save to file
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

    if (S_ISDIR(statdata.st_mode)){ //Path is a directory

        if (options.o_command)
            kill(options.parent_id, SIGUSR1); //Signal Directory

        DIR *c_dir;
        struct dirent *dir;
        char temp_filename[257];
        if ( (c_dir = opendir(filepath)) == NULL ){ //Failed to open directory
            perror("Error");
            exit(1);
        }

        while ( (dir = readdir(c_dir)) != NULL) { //Read all files in the directory first

            struct stat temp_stat;
            sprintf(temp_filename, "%s/%s", filepath, dir->d_name);

            //Check if path exists
            if (stat(temp_filename, &temp_stat) < 0) {
                perror("Error");
                exit(1);
            }

            //Check if regular file
            if (S_ISREG(temp_stat.st_mode))
                analyze_file(temp_filename, &temp_stat);
            else if (S_ISDIR(temp_stat.st_mode) && dir->d_name[0] != '.') {
                if (fork() > 0) {
                    continue;
                } else {
                    analyze_path(temp_filename);
                    exit(0);
                }
            }
        }
    }
    else {//Path is a file
        analyze_file(filepath, &statdata);
    }
}

int main (int argc, char *argv[]){
    //Signal Handler installer
    struct sigaction action;
    action.sa_handler = sig_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    if ( (sigaction(SIGINT,&action,NULL) < 0) || (sigaction(SIGUSR1,&action,NULL) < 0) || (sigaction(SIGUSR2,&action,NULL) < 0) ) {
        fprintf(stderr,"Unable to install signal handler\n");
        exit(1);
    }

    //Initiate options struct values
    options.hashmode = 0b000;
    options.o_command = NULL;
    options.r_command = false;
    options.v_command = false;
    options.mac_mode = false;
    options.parent_id = getpid();

    //Save program launch time
    gettimeofday(&options.start_time, NULL);

    //Parse the command arguments
    parse_arguments(argc, argv, &options);

    //Get the log file name from the env variable, create it when missing
    if (options.v_command){
        if ( (options.log_filepath = getenv("LOGFILENAME")) == NULL) {
            printf("-v: Can not generate log, environment variable \"LOGFILENAME\" does not exist.\n");
            exit(1);
        }
        remove(options.log_filepath);
    }

    //Log initial command
    if (options.v_command) {
        char log_msg[255] = "COMMAND ";
        for (int i = 0; i < argc; ++i) {
            strcat(log_msg, argv[i]);
            strcat(log_msg, " ");
        }
        log_command(log_msg);
    }

    //Filepath depending on mac or linux
    char* filepath;
    filepath = options.mac_mode ? argv[argc-2] : argv[argc-1];

    //Start analyzing
    analyze_path(filepath);

    while (wait(NULL) > 0); //All Directories in parallel, wait for all children to terminate

    //Print exit messages
    if (options.o_command != NULL)
        printf("Data saved on file %s\n", options.o_command);
    if (options.v_command)
        printf("Execution records saved on file %s\n", options.log_filepath);

    exit(0);
}
