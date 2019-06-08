/**
 * getenv() function searches the environment list to find the
 * environment variable name, and returns a pointer to the 
 * corresponding value string.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv, char **envp)
{
    printf("Hello %s !\n", getenv("USER"));
    return 0;
}

