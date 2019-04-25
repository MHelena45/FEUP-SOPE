all: user server

user: banking_aux.c user.c log.c
	gcc -Wall -Wextra banking_aux.c user.c log.c -o user

server: banking_aux.c server.c log.c
	gcc -Wall -Wextra banking_aux.c server.c log.c -o server

clean:
	rm -f *.o user
	rm -f *.o server
	