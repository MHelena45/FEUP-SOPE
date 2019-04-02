#define MAXLINE 512
#define MD5 0b001
#define SHA1 0b010
#define SHA256 0b100
#include <stdint.h>

typedef struct {
    uint8_t hashmode;
    char* o_command; //-o flag, save data in CSV file, string with filename, null if not used
    bool r_command;	//-r flag, analyse all files and subdirectories
    bool v_command;	//-v flag, generate log file, log file name in enviromnent variable LOGFILENAME
    bool mac_mode;
    int parent_id;
    char *log_filepath;
    struct timeval start_time;
} Options;


void append_to_file(char *message, char *filepath);

void executeSystemCommand(char *command, char *arguments ,char *result);

bool checkHashMode(char *hashMode, uint8_t *hash_option);

void stripHashCodeFromResult(char *hashCodeResult, char *stripedHash);

void parse_arguments(int argc, char **argv, Options *options);

void convert_timespec_to_string(time_t *org, char *dest);



