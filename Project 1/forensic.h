#include <signal.h>

#define MD5 0b001;
#define SHA1 0b010;
#define SHA256 0b100;

struct Options {
    uint8_t hashmode;
    char* o_command; //-o flag, save data in CSV file, string with filename, null if not used
    bool r_command;	//-r flag, analyse all files and subdirectories
    bool v_command;	//-v flag, generate log file, log file name in enviromnent variable LOGFILENAME
};

void sigint_handler(int signo) ;

/**
 * 
 */
int analyze_file (char *filepath, struct stat *statdata);

/**
 * 
 * 
 * 
 */
int analyze_path (char *filepath);