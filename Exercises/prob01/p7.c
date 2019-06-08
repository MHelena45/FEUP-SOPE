#include <stdio.h>
#include <stdlib.h>

void exithandler1() {
	printf("Executing exit handler 1\n");
}

void exithandler2() {
	printf("Executing exit handler 2 an exiting \n");
    exit(0);
}

void exithandler3() {
	printf("Executing exit handler 3\n");	
}

int main() {
	if(atexit(exithandler1) != 0) {
		printf("Error registering handler 1\n");
	}

	if(atexit(exithandler2) != 0) {
		printf("Error registering handler 2\n");
	}
	
	if(atexit(exithandler3) != 0) {
		printf("Error registering handler 3\n");
	}

	if(atexit(exithandler2) != 0) {
		printf("Error registering handler 2\n");
	}

    if(atexit(exithandler3) != 0) {
		printf("Error registering handler 3\n");
	}

    //abort();
    //_Exit(3);
    
	printf("Main done!\n");
    
	exit(0);
	//return 0;
}
/*
    As rotinas dos handlers são executadas por ordem inversa do seu resgisto
    Neste caso:
    Main done!
    Executing exit handler 3
    Executing exit handler 2 an exiting
    Executing exit handler 3
    Executing exit handler 2 an exiting
    Executing exit handler 1

    É possivel instalar um handler mais de uma vez.
    Se fizermos abort() ou _Exit() antes de "Main done!" nada será executado
    Se algum dos handlers terminar com exit() nada diferente acontecerá
*/