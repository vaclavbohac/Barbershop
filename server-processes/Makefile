server: server.o main.c
	cc -o server server.o main.c

server.o: server.h server.c
	cc -c server.h server.c 

clean:
	rm server.o server
