#include <signal.h>
#include <time.h>

#define MD5CMD "md5sum "
#define SHA1CMD "sha1sum "
#define SHA256CMD "sha256sum "
#define MD5CMDMAC "md5 "
#define SHA1CMDMAC "shasum -a 1 "
#define SHA256CMDMAC "shasum -a 256 "


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


