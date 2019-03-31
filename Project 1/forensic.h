#include <signal.h>
#include <time.h>
#include "forensic_aux.h"

#define MD5CMD "md5sum "
#define SHA1CMD "sha1sum "
#define SHA256CMD "sha256sum "
#define MD5CMDMAC "md5 "
#define SHA1CMDMAC "shasum -a 1 "
#define SHA256CMDMAC "shasum -a 256 "

struct Options {
    uint8_t hashmode;
    char* o_command; //-o flag, save data in CSV file, string with filename, null if not used
    bool r_command;	//-r flag, analyse all files and subdirectories
    bool v_command;	//-v flag, generate log file, log file name in enviromnent variable LOGFILENAME
    bool mac_mode;
    int parent_id;
    char log_filepath[255] ;
    struct timeval start_time;
};

void sig_handler(int signo) ;


void getAllHashModes(char *fileChar, char *result);


/**
 *
 */
void analyze_file (char *filepath, struct stat *statdata);

/**
 *
 *
 *
 */
void analyze_path (char *filepath);

void log_command(char *command);
