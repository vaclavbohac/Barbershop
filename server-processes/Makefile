server: server.o help.o main.c
	cc -o server server.o help.o main.c

server.o: server.h server.c
	cc -c server.h server.c 

help.o: help.h help.c
	cc -c help.h help.c

clean:
	rm server.o help.o server
