all: server

BIN = server
INC = messages/

server: server.o help.o main.c builder.o process.o
	cc -o $(BIN) server.o help.o main.c builder.o process.o

server.o: server.h server.c
	cc -c server.h server.c 

builder.o:
	@(cd $(INC); $(MAKE); mv builder.o ../)

help.o: help.h help.c
	cc -c help.h help.c

process.o: process.h process.c
	cc -c process.h process.c

clean:
	rm *.o *.gch $(BIN)
