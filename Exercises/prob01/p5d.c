#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char * fullname = getenv("USER_NAME");
	printf("Hello %s !\n", fullname);
	return 0;

}