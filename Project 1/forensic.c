#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main (int argc, char *argv[], char *envp[]){

	//File or Dir path
	char* filepath;
	//-r flag, analyse all filesand subdirectories
	bool r_command = false;
	//-h flag, string with algorithms, null if not used
	char *h_command = NULL;
	//-v flag, generate log file, log file name in enviromnent variable LOGFILENAME
	bool v_command = false;
	//-o flag, save data in CSV file, string with filename, null if not used
	char* o_command = NULL;
	
	/* Needs at least one argument */
	if (argc < 2){
		printf("forensic [-r] [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-v] <file|dir>\n");
		return -1;
	}
	
	/* Parse arguments */
	for (int i = 1; i < argc - 1; ++i){
		if (!strcmp(argv[i], "-r")){
			r_command = true;
		}
		// TO-DO: Verify if correct argument(md5, sha1, sha256)
		else if (!strcmp(argv[i], "-h")){
			if (i < argc - 2){
				h_command = argv[i+1];
				++i;
			}
			else {
				printf("forensic [-r] [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-v] <file|dir>\n");
				return -1;
			}
		}
		else if (!strcmp(argv[i], "-v")){
			v_command = true;
		}
		else if (!strcmp(argv[i], "-o")){
			if (i < argc - 2){
				o_command = argv[i+1];
				++i;
			}
			else {
				printf("forensic [-r] [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-v] <file|dir>\n");
				return -1;
			}
		}
	}
	
	filepath = argv[argc-1];
	
	//DEBUG
	printf("r: %d\n", r_command);
	if (h_command == NULL)
		printf("h: 0\n");
	else printf("h: %s\n", h_command);
	printf("v: %d\n", v_command);
	if (o_command == NULL)
		printf("o: 0\n");
	else printf("o: %s\n", o_command);
	printf("filepath: %s\n", filepath);
	
	return 0;	
}