    
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[], char* envp[]) {
	for(; *envp != NULL; envp++) {
		if(!strncmp(*envp, "USER=", strlen("USER="))) {
			printf("Hello %s !\n", *envp + strlen("USER="));
			return 0;
		}
	}
	printf("Error: USER environment variable not set!\n");
	return 1;
}