#include <stdio.h>

int main(void)
{
    char name[30]; 
    int number;
    printf("What's your name? \n");
    //scanf("%s", name);
    fgets(name, sizeof(name), stdin);

    //removes new line putted by fgets
    for(int i= 0; i < 29; i++){
        if(name[i] == '\n')
            name[i] = ' ';
    }
    printf("Repeat how many times? \n");
    scanf("%i", &number);
    for(int i = 0; i < number; i++)
        printf("Hello %s! \n", name);
    return 0;
} 
