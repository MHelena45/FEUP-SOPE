all: user server

user: source/banking_aux.c source/user.c source/log.c source/general_aux.c
	gcc -Wall -Wextra source/banking_aux.c source/user.c source/log.c source/general_aux.c -o user -lpthread

server: source/banking_aux.c source/server.c source/log.c source/general_aux.c 
	gcc -Wall -Wextra source/banking_aux.c source/server.c source/log.c source/general_aux.c -o server -lpthread

clean:
	rm -f *.o user
	rm -f *.o server
	