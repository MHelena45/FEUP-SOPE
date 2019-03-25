#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

void signal_handler(int signo) //handler do sinal
{
    sleep(10);
    exit(0);

} 

int analyze_file (char *filepath) {
	/*
		Comando stat(man 2 stat), da um struct com file_size(st_size), file_access(st_mode), 
		file_modification_date(st_mtime), file_created(st_ctime, last status change?)	
		Comando file da file_type(?)
		
		TO-DO:
			-chamar comando file, para obter file type
			-Formatar informaçao st_mode, (owner, group, others?)
			-Calcular md5, sha1, sha256 quando necessario
			-printf da informaçao ou gravar em ficheiro quando o_commmand diferente de null
	*/
	
	struct stat statdata;
	struct tm ct, mt;
	char *creation_time = (char*) malloc (19);
	char *mod_time = (char*) malloc (19);
	
	if (stat(filepath, &statdata) < 0){
		perror("Error");
		exit(1);	
	}
	
	localtime_r(&statdata.st_ctime, &ct);
	localtime_r(&statdata.st_mtime, &mt);
	sprintf(creation_time, "%d-%d-%dT%d:%d:%d", 1900 + ct.tm_year, ct.tm_mon, ct.tm_mday, ct.tm_hour, ct.tm_min, ct.tm_sec);
	sprintf(mod_time, "%d-%d-%dT%d:%d:%d", 1900 + mt.tm_year, mt.tm_mon, mt.tm_mday, mt.tm_hour, mt.tm_min, mt.tm_sec);
	
	//DEBUG
	printf("file_name,file_type,%ld,file access (parse st_mode),%s,%s\n", statdata.st_size, creation_time,mod_time);
	
	free(creation_time);
	free(mod_time);
	
	return 0;
	
	
}

int main (int argc, char *argv[], char *envp[]){

	struct sigaction action;
    action.sa_handler = signal_handler;
    sigemptyset(&action.sa_mask);

    if (sigaction(SIGINT,&action,NULL) < 0) //tratamento de sinal associado ao ctrl+c
    {
		fprintf(stderr,"Unable to install SIGTERM handler\n");
		exit(1);
    }
	
	char* filepath;			//File or Dir path
	bool r_command = false;	//-r flag, analyse all files and subdirectories
	char *h_command = NULL;	//-h flag, string with algorithms, null if not used
	bool v_command = false;	//-v flag, generate log file, log file name in enviromnent variable LOGFILENAME
	char* o_command = NULL;	//-o flag, save data in CSV file, string with filename, null if not used
		
	if (argc < 2){	/* Needs at least one argument */
		printf("forensic [-r] [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-v] <file|dir>\n");
		exit(1);
	}
	
	for (int i = 1; i < argc - 1; ++i){	/* Parse arguments */
		
		if (!strcmp(argv[i], "-r")){
			r_command = true;
			continue;
		}
		
		// TO-DO: Verify if correct argument(md5, sha1, sha256)
		if (!strcmp(argv[i], "-h")){
			if (i < argc - 2){
				h_command = argv[i+1];
				++i;
				continue;
			}
			else {
				printf("forensic [-r] [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-v] <file|dir>\n");
				exit(1);
			}
		}
		
		if (!strcmp(argv[i], "-v")){
			v_command = true;
			continue;
		}
		
		if (!strcmp(argv[i], "-o")){
			if (i < argc - 2){
				o_command = argv[i+1];
				++i;
				continue;
			}
			else {
				printf("forensic [-r] [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-v] <file|dir>\n");
				exit(1);
			}
		}
		
	}
	
	filepath = argv[argc-1];
	analyze_file(filepath);//Testar um ficheiro para ja
}
