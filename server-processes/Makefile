all: server

BIN = server
INC = messages/

server: server.o help.o main.c builder.o
	cc -o $(BIN) server.o help.o main.c builder.o

server.o: server.h server.c
	cc -c server.h server.c 

builder.o:
	@(cd $(INC); $(MAKE); mv builder.o ../)

help.o: help.h help.c
	cc -c help.h help.c

clean:
	rm *.o *.gch $(BIN)
