#define MAXLINE 512
#define MD5 0b001
#define SHA1 0b010
#define SHA256 0b100
#include <stdint.h>


void append_to_file(char *message, char *filepath);

void executeSystemCommand(char *command, char *result);

bool checkHashMode(char *hashMode, uint8_t *hash_option);

void stripHashCodeFromResult(char *hashCodeResult, char *stripedHash);
