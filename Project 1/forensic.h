#include <signal.h>

#define MAXLINE 512
#define MD5 0b001
#define SHA1 0b010
#define SHA256 0b100

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
};

void sig_handler(int signo) ;

void executeSystemCommand(char *command, char *result);

void stripHashCodeFromResult(char *hashCodeResult, char *stripedHash);

void getAllHashModes(char *fileChar, char *result);

bool checkHashMode(char *hashMode);
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

void append_to_file(char *message, char *filepath);

