all: user server

user: banking_aux.c user.c log.c general_aux.c
	gcc -Wall -Wextra banking_aux.c user.c log.c general_aux.c -o user

server: banking_aux.c server.c log.c general_aux.c 
	gcc -Wall -Wextra banking_aux.c server.c log.c general_aux.c -o server -lpthread

clean:
	rm -f *.o user
	rm -f *.o server
	