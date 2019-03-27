#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

/** Quando recebe o comando ctrl+c o programa e todos os processos filhos tem que terminar quando 
  * ja nao tiver operaçoes pendentes.  
  */	
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